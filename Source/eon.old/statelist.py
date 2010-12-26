##-----------------------------------------------------------------------------------
## eOn is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## A copy of the GNU General Public License is available at
## http://www.gnu.org/licenses/
##-----------------------------------------------------------------------------------

""" The statelist module. """

import logging
logger = logging.getLogger('statelist')
import math
import os
import shutil
import sys

from ConfigParser import SafeConfigParser 

import atoms
import config
import state


class StateList:
    """ The StateList class.  Serves as an interface to State objects and StateList metadata. """
    def __init__(self, state_path, epsilon_e, epsilon_r, use_identical, StateClass,
                 list_search_results=None, initial_state = None):
        ''' Check to see if state_path exists and that state zero exists.
            Initializes state zero when passed a initial_state only if state
            zero doesn't already exist. '''

        self.path = state_path
        self.epsilon_e = epsilon_e
        self.epsilon_r = epsilon_r
        self.use_identical = use_identical
        self.StateClass = StateClass
        self.list_search_results = list_search_results

        # Paths
        self.state_table_path = os.path.join(self.path, "state_table")

        # Create the statelist directory if it does not exist.
        if not os.path.isdir(self.path):
            logger.warning("state list path does not exist, creating %s" % self.path)
            os.makedirs(self.path)
            open(self.state_table_path, 'w').close()

        # Create the zero state directory if it does not exist.
        if not os.path.isdir(os.path.join(self.path, "0")):
            if initial_state == None:
                raise IOError("Missing zeroth state directory and no reactant provided.")
            self.StateClass(statepath = os.path.join(self.path, "0"), 
                        statenumber = 0, 
                        statelist = self,
                        previous_state_num = -1,
                        reactant_path = initial_state)

        # Other class variables.
        self.states = {}
        self.state_table = None

    def get_num_states(self):
        """ Returns the number of lines in the state_table file. """
        self.load_state_table()
        return len(self.state_table)

    def get_product_state(self, state_number, process_id):
        ''' Returns a State object referenced by state_number and process_id. '''
        #TODO: Compare configuration of product with existing states.

        # If the number of states in state_table is zero
        #we need to add the zero state and energy to the state table.
        if self.get_num_states() == 0:
            zst = self.get_state(0)
            self.append_state_table(zst.get_energy())

        # Load the state object containing the process we want the product for.
        st = self.get_state(state_number)
        st.load_process_table()

        # Get the state number for the product.
        newstnr = st.procs[process_id]['product']

        # If the product id is not initialized, make sure it is not a copy of an existing state.
        # Otherwise, create it, connect it to st, and return it.
        if newstnr == -1:

            # Make a list of states for which we need to compare configurations.
            enew = st.procs[process_id]['product_energy']
            energetically_close = []
            for id in range(self.get_num_states()):
                if abs(self.get_state(id).get_energy() - enew) < self.epsilon_e:
                    energetically_close.append(id)

            # Perform distance checks on the energetically close configurations.
            if len(energetically_close) > 0:
                pnew = st.get_process_product(process_id)
                for id in energetically_close:
                    p = self.get_state(id).get_reactant()
                    if self.use_identical:
                        if atoms.identical(p, pnew, self.epsilon_r):

                            # Update the reactant state to point at the new state id.
                            self.register_process(st.number, id, process_id)                            
                            return self.get_state(id)
                    else:
                        dist = max(atoms.per_atom_norm(p.r - pnew.r, p.box))
                        if dist < self.epsilon_r:

                            # Update the reactant state to point at the new state id.
                            self.register_process(st.number, id, process_id)                            
                            return self.get_state(id)

            # The id for the new state is the number of states.
            newstnr = self.get_num_states()

            # Create the new state object.
            newst = self.StateClass(statepath = self.state_path(newstnr), 
                                statenumber = newstnr, 
                                statelist = self,
                                previous_state_num = state_number,
                                reactant_path = st.proc_product_path(process_id)) 
            self.register_process(st.number, newstnr, process_id)

            # Append the new state to the state table.
            self.append_state_table(st.procs[process_id]['product_energy'])

        # The product state already exists, so get it.
        else:
            newst = self.get_state(newstnr)

        # Return the product state.
        return newst

    def get_state(self, state_number):
        ''' Returns a state object. '''
        if state_number in self.states:
            return self.states[state_number]
        st = self.StateClass(statepath = os.path.join(self.path, str(state_number)), 
                         statenumber = state_number, 
                         statelist = self)
        self.states[state_number] = st
        return st

    def load_state_table(self):
        if self.state_table == None:
            f = open(self.state_table_path, 'r')
            lines = f.readlines()
            f.close()
            self.state_table = []
            for l in lines:
                self.state_table.append(float(l.strip().split()[1]))

    def save_state_table(self):
        if self.state_table != None:
            f = open(self.state_table_path, 'w')
            for i in range(len(self.state_table)):
                f.write("% 7d %16.5f\n" % (i, self.state_table[i]))
            f.close()

    def append_state_table(self, energy):
        number = self.get_num_states()
        f = open(self.state_table_path, 'a')
        f.write("% 7d %16.5f\n" % (number, energy))
        f.close()
        if self.state_table != None:    
            self.state_table.append(energy)

    def state_path(self, state_number):
        """ Utility function to return the compiled path of a state, whether it exists or not. """
        return os.path.join(self.path, str(state_number))

if __name__ == "__main__":
    pass