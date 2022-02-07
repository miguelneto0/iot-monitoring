# iot-monitoring
 READme in progress . . .
This project was performed together @ElisianePaixao
An IoT project using MQTT protocol by Publisher-Subsribe method with Arduino UNO coupled with a DHT11 sensor (as Publisher) that monitors the temperature of cities and compares them using the Node-RED platform and Cloud Services (as Subscribers).

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/presentation.png"  width="680">
<p align = "left"><sup>
Image 1 (Architecture and Components)</sup></p>

## 1 [Description]

This project was performed together with @ElisianePaixao, the master's student at Electrical Engineer - UFSC. We propose a weather monitoring system of cities to be presented in a discipline of the Automation and Systems Engineer Postgraduate Program. The main idea is to integrate at Node-RED platform, the IBM Watson or Google Firebase services to store data monitored by an Arduino UNO through the MQTT protocol by Publisher-Subscribe method.

For the sake of simplicity, we outline the linked technologies and how they can be processed in the figure titled 'architecture.png'. The technologies used are presented in Section 2 and the details from the architecture are described in Section 3. In Section 4, the Palettes that you need to run this project are demonstrated. Besides, the Arduino code is available in the DHT folder, the configuration of components and sensors used are described in Architecture's Section and better explained in the .pptx file. The flow can be imported for Node-RED through a copy of the JSON file.

## 2 [Archictecture]

## 3 [Technologies]

- Node-RED platform ([about](www.nodered.org))
- Arduino UNO ([about](www.arduino.cc/en/Tutorial/HomePage))
- DHT Sensor ([about](www.arduino.cc/reference/en/libraries/dht-sensor-library/))
- Mosquitto MQTT ([about](www.mosquitto.org/))
- IBM Watson IoT ([about](www.ibm.com/cloud/watson-iot-platform))
- Open Weather ([about](www.openweathermap.org/api))

First, it is necessary install node-red and mosquitto software for execute the application remotely.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/arduino_sketch.png"  width="550">
<p align = "left"><sup>
Image 2 (Arduino Sketch for collect data with DTH11 sensor)</sup></p>

Install Node.js first, and after install Node-RED by command:
<code>npm install -g --unsafe-perm node-red</code>
Then, to run the Node-RED just type node-red in terminal and press Enter. If any error happens, the application can be accessed by localhost:1880/ in browser.

In following, the Mosquitto Broker should be installed. For this, download the mosquitto file in the [site](https://mosquitto.org/download/) as the operation system. And then, verify if mosquitto is on:
by Windows Services or <code>netstat -an</code>, and check localhost (127.0.0.1):1883/ is LISTENING.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/nodered_flow.png" width="680">
<p align = "left"><sup>
Image  (Dashboard in Node-RED)</sup></p>

## 4 [Palettes]

To run this project on the Node-RED platform, you need to install each palette after Node-RED to be online. In the right menu, selects the option 'Manage Palettes' and then the guide 'install'. Seek and install for each following libraries:
- node-red-node-arduino ( to include I/O ports for arduino communicate )
- node-red-node-serialport ( to include I/O alternative ports for arduino )
- node-red-node-openweathermap ( to get real time data from OpenWeather API )
- node-red-dashboard ( to show data )
- node-red-contrib-ibm-watson-iot ( to include a linked component IBM )

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/nodered_palettes.png" width="480">
<p align = "left"><sup>
Image  (Palettes installed in Node-RED via Manage Palette)</sup></p>

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/openweather_print.png" width="480">
<p align = "left"><sup>
Image  (OpenWeather config)</sup></p>

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/openweather_config.png" width="480">
<p align = "left"><sup>
Image  (OpenWeather config)</sup></p>

## Dashboard

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/dashboard_path.png" width="680">
<p align = "left"><sup>
Image  (Dashboard in Node-RED)</sup></p>
