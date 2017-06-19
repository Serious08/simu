from scenario import *

class Automate():
  def __init__(self, ob_scr, bb_scr):
    self.ob_scr = IpBasicScenario()
    self.bb_scr = IpBasicScenario()
    self.ob_scr.loadScr(ob_scr)
    self.bb_scr.loadScr(bb_scr)

  def start(self):
    self.ob_scr.start()
    self.ob_scr.onReceivedAck('DS02')
#    self.ob_scr.onReceivedAck('DS03')

#      self.bb_scr.start()
#      self.bb_scr.onReceivedMsg('DS02')
#      self.bb_scr.onReceivedAck('DS03')
#      self.bb_scr.onReceivedMsg('DR')
