from hoomd_plugins.EndtoEndAutoCorrelation_plugin import _EndtoEndAutoCorrelation_plugin

# Next, since we are extending an updater, we need to bring in the base class updater and some other parts from 
# hoomd_script
from hoomd_script.analyze import _analyzer
from hoomd_script import util
from hoomd_script import globals
import hoomd

class EndtoEndAutoCorrelation(_analyzer):
    def __init__(self, filename, chainlength, endtime, period=1, overwrite=False):
        util.print_status_line();
    
        # initialize base class
        _analyzer.__init__(self);
        self.cpp_analyzer = _EndtoEndAutoCorrelation_plugin.EndtoEndAutoCorrelation(globals.system_definition, filename, chainlength, endtime, overwrite);
        self.setupAnalyzer(period)
