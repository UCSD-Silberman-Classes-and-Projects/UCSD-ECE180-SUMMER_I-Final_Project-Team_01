from arduino.app_utils import App, Bridge
from arduino.app_bricks.web_ui import WebUI

print("--- Safe Interactive Dispenser Server Initiated ---")
ui = WebUI()
current_status = "Ready to Assist"

def get_plant_data():
    """Provides sensory updates directly to your interactive frontend container
framework."""

    global current_status
    try:
        m = Bridge.call("fetchM")
        r = Bridge.call("fetchR")
        # FIXED: Safety checkpoint updated to 440 to catch levels below 20% volume
        if r >= 440:
            current_status = "Water reservoir is empty please refill"
        else:
            if current_status == "Water reservoir is empty please refill":
                current_status = "Ready to Assist"
        return {
            "moisture": str(m),
            "reservoir": str(r),
            "status": str(current_status)
        }
    
    except Exception as e:
        return {
            "moisture": "0",
            "reservoir": "0",
            "status": f"Syncing hardware bridge... {e}"
        }
    
def run_on():
    print("-> Web Request: Engaging hardware contacts (ON)")
    Bridge.call("manualLedOn")
    return {"state": "ON"}

def run_off():
    print("-> Web Request: Releasing hardware contacts (OFF)")
    Bridge.call("manualLedOff")
    return {"state": "OFF"}

# Bind interaction paths
ui.expose_api("GET", "/api/data", get_plant_data)
ui.expose_api("GET", "/api/timed/on", run_on)
ui.expose_api("GET", "/api/timed/off", run_off)

App.run()