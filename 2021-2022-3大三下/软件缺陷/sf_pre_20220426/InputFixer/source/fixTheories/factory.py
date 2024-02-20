"""
This file generates an object of the appropriate theory.

IT MUST BE MODIFIED FOR ANY ADDED THEORY
ANY ADDED THEORY MUST IMPLEMENT THE .fix method
"""

#import fixTheories.gInFix as gInFix
import fixTheories.stateMachineHailMary as sm
def get_theory_solver(theory_name, global_config, log):
    """
    This is the factory function - ya, I know this is hacky, but....
    """

    if theory_name == "gInFix": 
        return gInFix.GInFix()
    elif theory_name == 'StateMachine':
        return sm.StateMachine(global_config, log)
    
    # TODO: Add custom theories here
    
    return None


