#!/usr/bin/python3
# -*- coding: utf-8 -*-

#expand path to find modules :
import sys
sys.path.append("..")
sys.path.append("../proto")
sys.path.append("../../devenv/nanopb-0.3.7-windows-x86/generator/proto")

import signal
import math
from PyQt5.Qt import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from gui import *

class TU_TableWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.table = TableWidget(self)



if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    app = QApplication(sys.argv)
    main = TU_TableWidget()
    main.show()
    sys.exit(app.exec_())
    
    
    