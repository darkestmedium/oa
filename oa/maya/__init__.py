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