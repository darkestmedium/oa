# Oa imports

# import oa.maya.Core
# import oa.maya.Anim
# import oa.maya.Render
# import oa.maya.Retarget
# import oa.maya.Rig
# import oa.maya.Ui

# import oa.maya.api

# Built-in imports


# Third-party imports

# Open APi imports



"""Open APi


Label imports:
	# Built-in imports:
		import subprocess as subp

	# Third-party imports:
		import numpy as np
		import PySide6.QtCore as qtc

	# Open APi imports:
		import api.Open as api
		import api.OpenUi as apiui

		import api.Open as oap
		import api.OpenUi as oapui


Naming:
	api.module.classfile.ClassName.MethodName()  Epic / OpenGl Convention

Open:
	api.io.file.FileIO.mkdir() -> api.io.file.iofile(FileIO).mkdir()
	api.io.data.DataIO.mkdir() -> api.io.data.iodata(DataIO).mkdir()
	import api.Open as oap
	oap.iofile.mkdir(path)

OpenUi:
	api.ui.ocvui.DrawOCVUi.draw() -> api.ui.ocvui.uicvdraw(DrawOCVUi).draw()
	import api.OpenUi as oapui
	oapui.ocvdraw.draw()


"""




# Constants




# C
class Color():
  Yellow        = [1.0, 0.6, 0.1]
  LightYellow   = [1.0, 1.0, 0.25]
  Orange        = [0.8, 0.25, 0.05]
  LightOrange   = [1.0, 0.467, 0.2]
  Blue          = [0.05, 0.25, 0.8]
  LightBlue     = [0.4, 0.8, 1.0]
  Magenta       = [0.6, 0.2, 0.4]
  Green         = [0.2, 0.8, 0.4]




class Lock():
  TransformsVisibility = ["translate", "rotate", "scale", "visibility"]

