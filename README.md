# Edge-AI Mobile Plant Assistant and Watering Station w/ Display

### ECE 180 Final Project
**Team 01 Summer Session I 2026**

---

## Team Members

* **Eric Yi** - EE, Machine Learning and Controls
* **Kathya Romano** - EE
* **Javier Avila** - EE

---

## Abstract

The Mobile Plant Assistant (MPA) is a distributed robotic system consisting of a mobile robot and a stationary water pump. A user positions the robot using a joystick to take a precise photo of a target plant. An AI model running on the robot's Arduino UNO Q analyzes the captured image and sends the analysis results to the stationary water pump. The stationary pump features an interactive webpage with options for Healthy, Wilted, and Dead; using the camera's feedback, the user selects the appropriate option. The machine then dispenses water only if needed, based on the plant's condition and live readings from a soil moisture sensor. The project utilizes two Arduino UNO Q boards communicating over a localized, private wireless network. The robot's board navigates to plants and uses a custom-trained ResNet18 computer vision model (deployed via ONNX and OpenCV) to classify plant health (Healthy, Wilted, Dead, or Background). These classifications are processed entirely on the edge device and transmitted via HTTP requests to a Flask server running on the  stationary water pump's board, which determines if the plant requires watering.

---

## What We Promised

### Must Have
* **Joystick Control:** The ability to precisely control the movement of the robot.
* **Robot Camera:** A live feed and high-quality camera mounted on the chassis for accurate monitoring.
* **Two Arduino Q's:** The robot's Arduino Q must send signals to the stationary pump for watering.
* **Stationary Device Display:** A machine that dispenses water according to the condition of the plant and moisture levels.
* **Moisture Sensor:** A sensor that plugs into the plant's soil and monitors the soil moisture levels.

### Nice to Have
* **Touch screen:** A touch screen for the water station for a more immersive experience without the need for a mouse.
* **Automated process:** Hand gesture control for the car, which was ultimately removed for easier debugging.

---

## Accomplishments

* **Accurate AI Classification:** The AI accurately determines the plant status (healthy, wilted, or dead) and successfully identifies when it is just looking at the background.
* **Working Mobile Robot:** The robot features precise controlling through a joystick system and contains a camera chassis mount designed for taking level-headed photos.
* **Water Irrigation System:** The water pump and moisture sensors work seamlessly with the webUI to dispense water according to the user's choice.
* **Plant Condition System:** The condition of the plant is accurately described and efficiently communicated to all required parts of the system.

---

## Challenges & Lessons Learned

* **Camera Latency & Hardware Limits:** The webcam turned out to be very laggy when attempting to make it a live feed, which forced us to change our Python code and overall workflow to take photos instead.
* **Out-of-Distribution AI Errors:** The AI model initially detected objects that weren't plants as plants because we only assigned three categories for training (dead, wilted, healthy) before recognizing the background as a necessary category.
* **Automation & Communication Failures:** Information from the primary Arduino (chassis and camera) was received but could not be used without manual human input. The webUI and water station failed to accurately retrieve the terminal information, leading to water not dispensing automatically.
* **Waterproofing Needs:** We learned to make sure to waterproof any component that could be damaged from moisture, especially the least replaceable and most important parts.

---

## Programs Written & How to Use

### Core Files
* `main.py` (Car Board): The primary execution loop for the mobile robot. It manages joystick inputs, captures a localized photo, runs the AI inference, and transmits the resulting status.
* `station_server.py` (Station Board): A lightweight application that hosts the interactive webpage and processes the manual trigger inputs from the user.
* `index.html`: The webUI interface featuring the Healthy, Wilted, and Dead operational buttons alongside the live soil moisture readings.
* `plant_classifier.onnx`: The frozen, deployed neural network.

### Usage Instructions
1. **Network Setup:** Connect both the Car Board and the Station Board to the shared local network.
2. **Start the Station:** Run `python3 station_server.py` on the stationary board to boot the water pump interface and launch the webUI.
3. **Launch the Car:** Execute the `main.py` script on the robot's Arduino UNO Q. 
4. **Operation:** Position the robot with the joystick to take a precise photo of the plant. Using the feedback provided by the camera, use the interactive webpage on the stationary pump to select the appropriate condition (Healthy, Wilted, or Dead) to dispense water.

---

## Presentation & Demonstration

* **Final Project Slides:** [View Presentation Slides](https://docs.google.com/presentation/d/1Oy7r3g3FmZLhDyl_FDwQoCNw7k2ziMkD8FnaqXkOBvw/edit?usp=sharing)
* **Demonstration Video:** [Watch Project Demos](https://drive.google.com/file/d/1dp30PCg77CTaF5y4aawQJ_mIqyjTzGJ7/view?usp=sharing)

---

## Acknowledgements

Special thank you to Professor Silberman and TA Jose Castillo for making this course awesome!

---

## Contacts

* **Eric Yi** - eeyi@ucsd.edu | https://www.linkedin.com/in/theericyi/
* **Kathya Romano** - [Email] | [LinkedIn]
* **Javier Avila** - [Email] | [LinkedIn]