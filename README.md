# Smart Patient Monitoring System

## 34365 IoT Prototyping Fall 23, DTU

An innovative healthcare system designed to enhance patient safety and improve emergency response times, that utilizes pressure, humidity, and temperature sensors to comprehensively monitor sleep activities, handling real-time alerts to notify responsible personnel, ensuring swift response to potential emergencies. 

## Project Poster
![Project poster](poster.png)

## Project schema
![Project schema](<project schema.png>)

## Technical description

This project deploys a smart low-power easy-integrable patient monitoring system based on LoraWan communication with the help of RN2483 transceivers. The system can come in handy for hospitals/healthcare facilities, where patient real-time monitoring and safety are of the major importance. Day and night supervision of patients' activity with the emphasis of gathering statistics related to sleep flow is something that the system can offer and, hence, simplify the medical staff duties. It is desgined so, that every patient's bed is supplied with an FSR (pressure sensor) and, for the sake of indoors conditions monitoring, temperature & humidity DHT11 sensor. The data is reported in real time with adjustable frequency to Azure IoT Central and DataCake mobile dashboard, thus making it easy to analyze patient's activity in both desktop and mobile modes. In case a patient has left his/her bed for more than defined, regulable offset period (for current version of codes included in this project there is no offset period - sparing time for demonstration puproses), the system perceives such a patient as missing. If this is the case, cloud-based logic integrated with LoraWan backend starts sending immediate downlink notifications, trigerring the buzzer situated close to, for instance, duty nurse, who is expected to take action in finding the missing patient. Arduino Pro-Mini 3,3V, 8MHz of Arduino family is chosen as hosting micro-controller on both transmitter and receiver sides. While buzzer is signalling alarm, the button could be pressed for disabling the buzzer and put it on hold for a regulable period. That saves energy of the receiver and stops the noise produced by the buzzer. When the patient is back on his/her bed, the receiver will get notification automatically and, therefore, will no longer be in an "alarm mode".

## Built with
*  LoRaWAN
*  Helium
*  Datacake
*  Azure IoT Central
*  Arduino

# Authors
Dmytro Kirshev, Adam Rogowski, Vajk Olivér Vozári, Lukás Málek, Alberto Benito Pontes, Cesare Maio. 
