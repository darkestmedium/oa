# Built-in imports
import sys
import telnetlib
import os
import platform
import subprocess
import time
import logging

# Third-party imports

# Custom imports
# import mayaquit



log = logging.getLogger("build-maya-plugin")
log.addHandler(logging.StreamHandler(sys.stdout))
log.setLevel(logging.DEBUG)




class MayaRemote():
  """Remote operations in maya.
  """
  port = 20240

  @classmethod
  def LoadPlugin(cls, pluginName:str):
    if sys.argv.__len__() > 1:
      cls.port = sys.argv[1]
    try:
      tn = telnetlib.Telnet("localhost", cls.port)
      tn.write(f'catchQuiet(`loadPlugin "{pluginName}"`)'.encode())
      tn.close()
    except:
      pass


  @classmethod
  def UnloadPlugin(cls, pluginName:str):
    if sys.argv.__len__() > 1:
      cls.port = sys.argv[1]
    try:
      tn = telnetlib.Telnet("localhost", cls.port)
      tn.write(f'catchQuiet(`loadPlugin "{pluginName}"`)'.encode())
      tn.close()
    except:
      pass


  @classmethod
  def Quit(cls):
    if sys.argv.__len__() > 1:
      cls.port = sys.argv[1]
    try:
      tn = telnetlib.Telnet("localhost", cls.port)
      tn.write('catchQuiet(`quit -force`)'.encode())
      tn.close()
    except:
      pass




class MayaPlugin():
  """Class for automating the development of a maya plugin.
  """
  def __init__(self,
    projectName:str="oa",
    pluginName:str="oam",
    mayaVersion:int=2024,
    buildType:str="Release",
    buildTarget:str="all",
    clean:bool=True,
  ) -> None:
    self.GetPlatformData()

    self.projectName=projectName
    self.mayaVersion=mayaVersion

    self.buildType=buildType
    self.buildTarget=buildTarget

    self.mayaName = "Maya"

    # Paths
    self.GetPaths()

    # Plugin Info
    self.pluginName = f"{pluginName}{self.pluginExt}"
    self.pathBuiltPlugin = f"{self.pathBuildMayaOsVersionType}/src/maya/{self.pluginName}"

    # Run commands
    if clean:
      log.info("Performing clean build, removing existing files...")
      # output = subprocess.run(["rm", "-rf", self.pathBuildMaya], capture_output=True, text=True)
      output = subprocess.run(["rm", "-rf", self.pathBuildMaya], capture_output=True, text=True)
      log.debug(output)
      log.info("Removed previous build files!")

    output = subprocess.run(["mkdir", "-p", self.pathBuildMayaOsVersionType], capture_output=True, text=True)
    output = subprocess.run(["cd", self.pathBuildMayaOsVersionType], capture_output=True, text=True)


    MayaRemote.Quit()

    # self.Build()
    print("Starting cmake configuration...")
    output = subprocess.run(
      [
        "cmake",
        f"-G {self.generator}",
        f"-DMAYA_VERSION={self.mayaVersion}",
        f"-DCMAKE_BUILD_TYPE:STRING={self.buildType}",
        self.pathProject,
      ],
      cwd=self.pathBuildMayaOsVersionType, capture_output=True, text=True, check=True
    )

    print("Starting cmake build...")
    output = subprocess.run(
      [
        "cmake",
        "--build", ".",
        "--config", self.buildType, 
      ],
      cwd=self.pathBuildMayaOsVersionType, capture_output=True, text=True, check=True
    )

    output = subprocess.run(["cp", "-f", self.pathBuiltPlugin, self.pathPluginTarget], capture_output=True, text=True)


    isAppRunning = False
    while not isAppRunning:
      # time.sleep(3)
      isAppRunning = subprocess.run(['pgrep', self.mayaName], capture_output=True)
      if isAppRunning.returncode == 0:
        print(f"{self.mayaName} is running")
        isAppRunning = True
      else:
        print(f"Launching {self.mayaName}")
        if self.namePlatform == "Linux": 
          subprocess.run(["maya"])
        if self.namePlatform == "Darwin": 
          subprocess.run([f"/Applications/Autodesk/maya{self.mayaVersion}/Maya.app/Contents/bin/maya"])



  @classmethod
  def GetPlatformData(cls):
    """Gets os specific build data.
    """
    cls.namePlatform = platform.system()
    if cls.namePlatform == "Darwin": 
      cls.nameOs = "macos"
      cls.generator = "Unix Makefiles"
      cls.pluginExt = ".bundle"
    if cls.namePlatform == "Linux": 
      cls.nameOs = "linux"
      cls.generator = "Unix Makefiles"
      cls.pluginExt = ".so"
    if cls.namePlatform == "Windows": 
      cls.nameOs = "windows"
      cls.generator = "Visual Studio 17 2022"
      cls.pluginExt = ".mll"


  def GetPaths(self):
    """Builds the path required for building and deploying targets.
    """
    self.pathProject = __file__.split(".")[0]
    self.pathBuild = f"{self.pathProject}/build"
    self.pathBuildMaya = f"{self.pathBuild}/maya"
    self.pathBuildMayaOs = f"{self.pathBuildMaya}/{self.nameOs}"
    self.pathBuildMayaOsVersion = f"{self.pathBuildMayaOs}/{self.mayaVersion}"
    self.pathBuildMayaOsVersionType = f"{self.pathBuildMayaOsVersion}/{self.buildType}"
    self.pathPluginTarget = f"{self.pathProject}{self.projectName}/maya/resources/plugins/{self.nameOs}/{self.mayaVersion}"


  def Build(self):
    """Launch CMake build commands.
    """
    print("Starting cmake configuration...")
    output = subprocess.run(
      [
        "cmake",
        f"-G {self.generator}",
        f"-DMAYA_VERSION={self.mayaVersion}",
        f"-DCMAKE_BUILD_TYPE:STRING={self.buildType}",
        self.pathProject,
      ],
      cwd=self.pathBuildMayaOsVersionType, capture_output=True, text=True, check=True
    )

    print("Starting cmake build...")
    output = subprocess.run(
      [
        "cmake",
        "--build", ".",
        "--config", self.buildType, 
      ],
      cwd=self.pathBuildMayaOsVersionType, capture_output=True, text=True, check=True
    )

    output = subprocess.run(["cp", "-f", self.pathBuiltPlugin, self.pathPluginTarget], capture_output=True, text=True)


    isAppRunning = False
    while not isAppRunning:
      # time.sleep(3)
      isAppRunning = subprocess.run(['pgrep', self.mayaName], capture_output=True)
      if isAppRunning.returncode == 0:
        print(f"{self.mayaName} is running")
        isAppRunning = True
      else:
        print(f"Launching {self.mayaName}")
        if self.namePlatform == "Linux": 
          subprocess.run(["maya"])
        if self.namePlatform == "Darwin": 
          subprocess.run([f"/Applications/Autodesk/maya{self.mayaVersion}/Maya.app/Contents/bin/maya"])








if __name__ == "__main__":

  # print(os.environ["mayaVersion"])

  MayaPlugin(
    # projectName="mlunar",
    mayaVersion=os.environ["mayaVersion"], 
    # buildType="Debug",
    # buildTarget="all",
    # clean=False,
  )
