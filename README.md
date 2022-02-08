# iot-monitoring
 
This project is part of an academic work for discipline of Implementation Techniques for Automatized Systems, and was performed together with @ElisianePaixao.

An IoT project using **MQTT** protocol by Publisher-Subscribe method with **Arduino UNO** coupled with a **DHT11 sensor** (as Publisher) that monitors the temperature of cities and compares them using the **Node-RED** platform and **Cloud Services** (as Subscribers). The Figure 1 below describe the components used in this project and the architecture designed.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/presentation.png"  width="680">
<p align = "left"><sup>
Image 1 (Architecture and Components)</sup></p>

## 1 [Description]

The proposal of this work consists of a weather monitoring system of cities to be presented in a discipline of the Automation and Systems Engineer Postgraduate Program. The main idea is to integrate at **Node-RED** platform, the **IBM Watson** or **Google Firebase** services to store real-time data monitored by an **Arduino UNO** through the MQTT protocol by Publisher-Subscribe method.

<!--For the sake of simplicity, we outline the linked technologies and how they can be processed in the architecture (described in Section 2). The technologies used are presented in Section 3 and, in following, the Section 4 and 5 details the settings for tools and APIs used in this IoT ecosystem. All images can be found in images folder, Arduino code is available in the DHT folder, the application flow can be imported for Node-RED through a copy of the JSON file available, and the configuration of components and sensors used are described and better explained in the .pptx file.) -->

## 2 [Archictecture]

In order to explain the project architecture, first, it is necessary to know how works **Publish-Subscribe** protocols, like **MQTT protocol**. In this protocol, the communication between nodes of the network is performed through the access of topics. A topic consists of a path through which the data of interest passes. In our case, a topic is represented as **"tisa/sensor"** and some data should be published on this topic for different components can access the data published in this topic. Thus, the publisher responsible for sending data for this topic is the **Arduino UNO** coupled with a **DHT11** sensor, that periodically (every 2 seconds) feeds the topic. Then, the components interested in these data should subscribe to this topic to access, in this case, the subscribers are in the **Node-RED** platform, such as the computer that access the data from **IBM Watson Cloud Service**. The intermediate component which is responsible for this communication is called broker, and here, we used the **MQTT broker (from Mosquitto app)**. The MQTT broker can intermediate the communication with cloud services and APIs as the **OpenWeather** used for comparisson in this project.

## 3 [Technologies]

- Node-RED platform ([about](www.nodered.org))
- Arduino UNO ([about](www.arduino.cc/en/Tutorial/HomePage))
- DHT Sensor ([about](www.arduino.cc/reference/en/libraries/dht-sensor-library/))
- Mosquitto MQTT ([about](www.mosquitto.org/))
- IBM Watson IoT ([about](www.ibm.com/cloud/watson-iot-platform))
- Open Weather ([about](www.openweathermap.org/api))

First, to use the node-red and mosquitto software for execute the application remotely, it is necessary install the Node.js from this [download link](https://nodejs.org/en/download/). After, to install the Node-RED, use the following command:

- <code>npm install -g --unsafe-perm node-red</code>

To run the Node-RED just type <code>node-red</code> in a terminal and press Enter. If there are no erros, the application can be accessed by **localhost:1880/** in the browser. In following, the **Mosquitto Broker** should be installed from the download of the mosquitto file in the [site](https://mosquitto.org/download/) as the operation system. And after that, it is important to verify if mosquitto is activated of two ways:

- (on Windows OS) by Windows Services: use the keyboard shortcut **WinKey + R** and inside of little window that appears, type **"services.msc"** and Enter. After, check if there is a service named Mosquitto and if its status is **RUNNING**.
- (via Terminal) type <code>netstat -an</code> and check localhost **(127.0.0.1):1883/** is **LISTENING**.

After that Node-RED and MQTT broker are work, we have to create a flow in the Node-RED via browser, using the method click and drag components such as the temperature, humidity and pressure Gauges, function nodes, messages nodes, and specific nodes for MQTT protocol, Arduino connection, IBM Watson components, for example. For these specific components we need to install Palettes corresponding to this technologies. The palettes are presented in Section 4.

## 4 [Palettes]

To run this project on the Node-RED platform, you need to install each palette after Node-RED to be online. In the right menu, selects the option 'Manage Palettes' and then the guide 'install'. Seek and install for each following libraries:
- node-red-node-arduino ( to include I/O ports for arduino communicate )
- node-red-node-serialport ( to include I/O alternative ports for arduino )
- node-red-node-openweathermap ( to get real time data from OpenWeather API )
- node-red-dashboard ( to show data )
- node-red-contrib-ibm-watson-iot ( to include a linked component IBM )

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/nodered_palettes.png" width="480">
<p align = "left"><sup>
Figure 1 (Palettes installed in Node-RED via Manage Palette)</sup></p>

## Nodes Settings

The implementation of Arduino code in C-like language was used just the DHT library to control the temperature and humidity data sensed. Basically, the <code>DHT.h</code> library provides a Class DHT which allows creating objects from the parameters: type (DHTTYPE) and the connected PIN in the Arduino (DHTPIN). And the **void setup()** function just defines the baud rate of the serial port (9600) and initializes the sensor with **dht.begin()**. Meanwhile, the **void loop()** just gets values from readTemperature and readHumidity to print and send to output which after is published in the topic, as Figure 3. 
 
<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/arduino_sketch.png"  width="550">
<p align = "left"><sup>
Figure 2 (Arduino Sketch for collect data with DTH11 sensor)</sup></p>

In the following, the flow of the nodes must be designed on the Node-RED as Figure 3 below. In this flow, we have on the top an Arduino node connected to MQTT publisher node (mosquittoPUB), both installed from the library by Manage Palette. After, the subscribers such as IBM Watson with temperature and humidity values and OpenWeather data are managed by the function nodes written in simple JavaScript code as the **dataproc_funcition_code.js** and **wheater_function_code.js** files. This function has as goal to discretize the values received from DTHsensor and prepare to store in cloud service of **IBM Watson**.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/nodered_flow.png" width="680">
<p align = "left"><sup>
Figure 3 (Nodes flow in Node-RED)</sup></p>

One of these nodes is a special type from OpenWeather, which can be set from the creation of an account in openweatherAPI and generate a KEY for each topic. After that, it is necessary to configure the OpenWeather node in Node-RED with this KEY and remainder parameter to integrate the API with the IoT system, as Figure 4 and 5.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/openweather_print.png" width="480">
<p align = "left"><sup>
Figure 4 (OpenWeather account)</sup></p>

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/openweather_config.png" width="480">
<p align = "left"><sup> Figure 5 (OpenWeather configuration)</sup></p>

In turn, to integrate the cloud service for storing data sensed every time, we use the IBM Watson, which needs to create an account for getting instances to manage the data and keep a history for a long time. The integration is shown in Figure 6.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/ibmwatson_config.png" width="680">
<p align = "left"><sup> Figure 6 (IBM Watson integration)</sup></p>

## Dashboard

Finally, the dashboard that shows the behavior of this system in real-time is presented in Figure 7 below. In this figure, it is possible to know how to open the dashboard from the node-red tools.

<img src="https://github.com/miguelneto0/iot-monitoring/blob/main/images/dashboard_path.png" width="680">
<p align = "left"><sup>
Figure 7 (Dashboard in Node-RED)</sup></p>
