import logging
import time
from threading import Timer,Thread,Event

logger = logging.getLogger('simu')

class IpBasicScenario():
  def __init__(self):
    self.name = ''
    self.t = None
    self.msg = []
    self.steps = []
    self.currentStep = 0
    self.nbSteps = 0
    self.cmds_list = ['SEND','RECEIVED','ACK']
    self.msg_type = ['DS02','DS03','DR']

  def onReceivedMsg(self, msg):
    logger.info("onReceivedMsg")
    valid, error = self.validateStep('RECEIVED', msg)
    if valid:
        self.currentStep = self.currentStep + 1
        self.nextStep()
    else:
      self.error(error)

  def onReceivedAck(self, msg):
    if self.t != None:
      self.t.cancel()
    valid, error = self.validateStep('ACK', msg)
    if valid:
      logger.info("onReceivedAck " + msg)
      self.currentStep = self.currentStep + 1
      self.nextStep()
    else:
      self.error(error)

  def sendMsg(self, msg):
    logger.info("onsendmsg " + msg)
    self.t = Timer(2, self.onTimeoutAck, [msg])
    self.t.start()
    self.currentStep = self.currentStep + 1

  def sendAck(self):
    logger.info("sendack")
    self.currentStep = self.currentStep + 1
    self.nextStep()

  def onTimeoutAck(self, msg):
    logger.info("timeout ack " + msg)
    self.error('timeour ack ' + msg)

  def nextStep(self):
    if self.currentStep < self.nbSteps:
      if self.steps[self.currentStep][0] == 'SEND':
        self.sendMsg(self.steps[self.currentStep][1])
      elif self.steps[self.currentStep][0] == 'ACK':
        self.sendAck()
      else:
        logger.info('Waiting msg')

  def validateStep(self, method, msg):
    error = ''
    if self.steps[self.currentStep][0] != method:
      error = 'ERROR STEP'
    if self.steps[self.currentStep][1] != msg:
      error = 'ERROR MSG TYPE'
    return error == '', error

  def start(self):
    logger.info('Scenario \'%s\' started', self.name)
    self.nextStep()

  def error(self, error):
    if self.t != None:
      self.t.cancel()
    logger.error('Scenario KO : ' + error)

  def loadScr(self, file):
    logger.info('Load scenario \'%s\'', file)
    with open(file, "r") as f:
      for line in f:
        cmd, msg = line.rstrip('\n\r').split(' ')
        if cmd in self.cmds_list and msg in self.msg_type:
          self.steps.append([cmd, msg])
          self.nbSteps = self.nbSteps + 1
        else:
          logger.error('can\'t load scenario ' + file)
          return
    logger.info('Scenario \'%s\' loaded', file)
    self.name = file


