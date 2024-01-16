



class HumanIk():
  """Retargeting with HumanIk in Maya, inherited from AbstractRetarget.

  Hik Character has the following nodes:
    HIKCharacterNode
    HIKProperty2State
    HIKSolerNode
    HIKState2SK
    HIKRetargeterNode

  HIKProperty2State.message -> propertyState.HIKCharacterNode.OutputCharacterDefinition -> InputCharacterDefinition.HIKSolerNode
                                                                  -> InputCharacterDefinition.HIKState2SK
                                                                  -> InputCharacterDefinitionDst.HIKRetargeterNode

  Validation order:
    __init__():
      initSetup():
        if isValid() ->	characterExists() -> isLocked() -> getPropertiesNode()
        if not valid ->	validateDefinition()

  TODO:
    bakeAnimation -> Add animationLayer support
    deleteAnimation -> ADD animationLayer support
    setSource -> ADD support for setting the stance, control rig option.
    setup mayaBatch -> create a gui independent mode (batch mode with ui scripts dependend

  """
  # minimalDefinition = lmrrhi.templateHik["minimalDefinition"]
  # definition = lmrrhi.templateHik["definition"]
  hikTemplate = "HumanIk"