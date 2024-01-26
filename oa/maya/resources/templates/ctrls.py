#--------------------------------------------------------------------------------------------------
# Ctrl Templates
#--------------------------------------------------------------------------------------------------



defaultTransparency = 0.15
defaultLineWidth = 2.0
defaultFingerLineWidth = 2.0
defaultLockShapeAttributes = False




colors = {
  "lightyellow":  [1.0, 1.0, 0.25],
  "yellow":       [1.0, 0.6, 0.1],
  "lightorange":  [1.0, 0.467, 0.2],
  "orange":       [0.8, 0.25, 0.05],
  "lightblue":    [0.4, 0.8, 1.0],
  "blue":         [0.05, 0.25, 0.8],
  "magenta":      [0.6, 0.2, 0.4],
  "green":        [0.2, 0.8, 0.4]
}




templates = {

  "base": {
    "shape": "base",
    "localPosition": (0.0,	0.0, 0.0), "localRotate": (0.0, 45.0, 0.0), "localScale": (100.0, 100.0, 100.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    # "lockShapeAttributes": True,
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "root": {
    "shape": "arrow",
    "localPosition": (0.0,	0.0, 0.0), "localRotate": (0.0, 0.0, 0.0),	"localScale": (15.0, 15.0, 15.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "pelvis": {
    "shape": "square",
    "localPosition": (0.0, 0.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (40.0, 40.0, 40.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "twist": {
    "shape": "square",
    "localPosition": (0.0,	0.0, 0.0), "localRotate": (0.0, 45.0, 0.0), "localScale": (4.0, 4.0, 4.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "finger": {
    "shape": "circle",
    "localPosition": (0.0,	0.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (3.0, 3.0, 3.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "corrective": {
    "shape": "locator",
    "localPosition": (0.0,	0.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (3.0, 3.0, 3.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "out": {
    "shape": "square",
    "localPosition": (0.0, 0.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (0.0, 0.0, 0.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "ik": {
    "shape": "cube",
    "localPosition": (0.0, 0.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (6.0, 6.0, 6.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["yellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "pv": {
    "shape": "locator",
    "localPosition": (0.0, 0.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (8.0, 8.0, 8.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": True,
    "lineWidth": defaultLineWidth,
    "color": colors["yellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

  "fkikswitch": {
    "shape": "diamond",
    "localPosition": (0.0, 25.0, 0.0), "localRotate": (0.0, 0.0, 0.0), "localScale": (2.0, 2.0, 2.0),
    "drawSolverMode": False, "solverModePosition": (0.0, 0.0, 0.0),
    "drawLine": False,
    "lineWidth": defaultLineWidth,
    "color": colors["lightyellow"],
    "lockShapeAttributes": defaultLockShapeAttributes,
  },

}