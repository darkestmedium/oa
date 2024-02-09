# Built-in imports
import logging

# Third-party imports
from maya import cmds
from maya import mel


menuName = "Speed Locator"
logger = logging.getLogger(menuName)



class MainMenu():
	"""Class for creating user-defined menu items in the maya main menu."""


	# GUI
	menuItems = []


	def createMenuItems(self) -> bool:
		"""Adds custom menu items in the maya main menu.

		Must be overridden in the derived class.

		Returns:
			bool: True if the operation was successful, False if an	error occured during the
				operation.

		"""
		return False


	def displayOptionBox(self, *args, **kwargs):
		"""Displays the option box window for the command.
		
		Must be overridden in the derived class.

		"""
		pass


	def applyAndCloseButton(self, *args, **kwargs):
		"""Calls the doIt method and closes the option box window by saving the values."""
		self.doIt()
		mel.eval('saveOptionBoxSize')
		self.closeOptionBox()


	def closeOptionBox(self, *args, **kwargs):
		"""Closes the option box window."""
		mel.eval('hideOptionBox')


	def resetToDefaults(self, *args, **kwargs):
		"""Resets the settings to default ones.

		Must be overridden in the derived class.

		"""
		pass


	def getCreateCommandKwargs(self, *args, **kwargs) -> dict:
		"""Gets the moduleTemplate command arguments.

		The arguments are queried eiter from the option box widgets or the saved option 
		variables. If the widgets exist, their values will be saved to the option variables.

		Must be overridden in the derived class.

		Returns:
			dict: Dictionary of the kwargs to the moduleTemplate command.

		"""
		pass


	def doIt(self, *args, **kwargs) -> bool:
		"""Wrapper method for the main menu item.

		Must be overridden in the derived class.

		Returns:
			bool: True if the operation was successful, False if an	error occured during the operation.

		"""
		return False


	def deleteMenuItems(self) -> bool:
		"""Deletes custom menu items in the maya main menu.

		Returns:
			bool: True if the operation was successful, False if an	error occured during the
				operation.

		"""
		if self.menuItems:
			for item in self.menuItems:
				cmds.deleteUI(item, menuItem=True)

			self.menuItems.clear()

			logger.debug(f"Successfully deleted '{menuName}' item from main menu.")
			return True

		logger.debug(f"'{menuName}' menu item not found, nothing to delete.")
		return False



class SpeedLocatorMainMenu(MainMenu):
	"""Derived class for the speedLocator plugin for creating user-defined menu items in the maya main menu."""


	def createMenuItems(self) -> bool:
		"""Adds custom menu items in the maya main menu.

		Returns:
			bool: True if the operation was successful, False if an	error occured during the
				operation.

		"""
		if len(self.menuItems) == 0:
			menu = "mainCreateMenu"
			mel.eval("ModCreateMenu mainCreateMenu;")
			items = cmds.menu(menu, query=True, itemArray=True)
			measureItemIndex = None
			for index, item in enumerate(items):
				if item == "measureItem":	measureItemIndex = index

			# Speed Locator
			speedLocatorItem = cmds.menuItem(
				parent=menu,
				insertAfter=items[measureItemIndex-2],
				label="Speed Locator",
				image="out_speedLocator.png",
				command=self.doIt,
				sourceType="python",
			)

			self.menuItems.append(speedLocatorItem)

		return True


	def doIt(self, *args, **kwargs) -> bool:
		"""Wrapper method for the main menu item.

		Returns:
			bool: True if the operation was successful, False if an	error occured during the
				operation.

		"""
		if cmds.speed(): return True

		return False
