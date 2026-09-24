// A simple script to send data to an Arduino
// Created by Byron Lahey on 9/23/2026  
// Requirements: Unity, Arduino, Serial communication   
// The first data source will be the position of a GameObject in the scene.
// The second data source will be the status of a Light in the scene (off or on). This will be sent as a binary value (0 or 1) using an integer.
// Use a timer to send data at regular intervals.   


using UnityEngine;
using System.IO.Ports;
using System;


public class SendToArduinoScript : MonoBehaviour
{
    public string portName = "COM3"; // Change this to your Arduino's port
    public int baudRate = 115200;
    private SerialPort serialPort;
    // Start is called once before the first execution of Update after the MonoBehaviour is created
   
    public Transform dataSource; // Drag the GameObject whose position you want to send to the Arduino

    public Light lightSource; // Drag the Light whose status you want to send to the Arduino
    public int sendInterval = 40; // The interval at which to send data (in milliseconds)




    void Start()
    {
        serialPort = new SerialPort(portName, baudRate);
        serialPort.WriteTimeout = 1000;
        try
        {
            serialPort.Open();

        }
        catch (Exception e)
        {
            Debug.LogError("Error opening serial port: " + e.Message);
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (Time.time % (sendInterval / 1000.0f) < Time.deltaTime / 1000.0f)
        {
            SendDataToArduino();
        }
    }

    void SendDataToArduino()
    {

        if (serialPort != null && serialPort.IsOpen)
        {
            // Example: Send the position of the dataSource GameObject
            Vector3 position = dataSource.position;
            string data = string.Format("{0},{1},{2},{3}", position.x, position.y, position.z, lightSource.enabled ? 1 : 0);

            serialPort.WriteLine(data);
        }
    }

    void OnApplicationQuit()
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
        }
    }
}
