Import("env")
use_bt = env.GetProjectOption("use_bluestack", "no")

if use_bt == "yes":
    env.Append(CPPDEFINES=["USE_BLUEPAD32"])
    env.Append(PIOPLATFORMPACKAGES=[
        "framework-arduinoespressif32@https://github.com/maxgerhardt/pio-framework-bluepad32/archive/refs/heads/main.zip"
    ])