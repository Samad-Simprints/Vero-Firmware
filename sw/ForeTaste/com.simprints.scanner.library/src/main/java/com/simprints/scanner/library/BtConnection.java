package com.simprints.scanner.library;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import static android.support.v4.app.ActivityCompat.startActivityForResult;

/**
 * Created by derek on 25/08/2015.
 */
public class BtConnection extends Connection
{
  private static final String TAG = "BtConnection";
  private static final UUID SPP_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
  private BluetoothAdapter mBluetoothAdapter = null;
  private BluetoothDevice btScanner = null;
  private BluetoothSocket btSocket = null;
  private OutputStream outStream = null;
  private InputStream inStream = null;

  public BtConnection()
  {
    super("Bluetooth Classic");
  }

  public void enable(Activity activity) {
    // ask user to enable BT, then try again later ..
    int REQUEST_ENABLE_BT = 1;
    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
    startActivityForResult(activity, enableBtIntent, REQUEST_ENABLE_BT, null);
  }

  public boolean init() {
    isSetup = false;

    mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    if (mBluetoothAdapter == null)
    {
      errorMessage = "Bluetooth unavailable";
    }
    else if (!mBluetoothAdapter.isEnabled())
    {
      // ask user to enable BT, then try again later ..
      int REQUEST_ENABLE_BT = 1;
      Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);

      errorMessage = "Bluetooth disabled";
    }
    else
    {
      Set<BluetoothDevice> bluetoothDevices;

      // look for device
      bluetoothDevices = mBluetoothAdapter.getBondedDevices();
      if (bluetoothDevices.size()>0)
      {
        Log.v(TAG,"BT devices found");
        for (BluetoothDevice device: bluetoothDevices)
        {
          Log.v(TAG,device.getName() + " " + device.getAddress());
          if (device.getName().equals("Adafruit EZ-Link 048b"))
          {
            btScanner = device;
          }
        }
        if (btScanner!=null)
        {
          Log.v(TAG, "Scanner found");
          isSetup = true;
        }
      }
      else
      {
        //@TODO: discovery goes here
      }
    }

    return isSetup;
  }

  public String deviceName()
  {
    return btScanner.getName();
  }

  public String deviceDetail() {
    return btScanner.getAddress();
  }

  @Override
  public void open(ConnectionCallback callback) {
    Boolean created = false;
    Boolean connected = false;

    super.open(callback);

    // discovery will slow down connection if on. Call is harmless if already off.
    mBluetoothAdapter.cancelDiscovery();

    if (isSetup)
    {
      try
      {
        btSocket = btScanner.createInsecureRfcommSocketToServiceRecord(SPP_UUID);
        created = true;
        Log.v(TAG, "Socket created");
      } catch (IOException e)
      {
        created = false;
        Log.v(TAG, "Error " + e.getMessage());
      }

      if (created)
      {
        try
        {
          btSocket.connect();
          connected = true;
          Log.v(TAG, "Socket connected");
        } catch (IOException e)
        {
          connected = false;
          Log.v(TAG, "Connect error " + e.getMessage());
          try
          {
            btSocket.close();
          } catch (IOException e2)
          {
            Log.v(TAG, "Close error " + e2.getMessage());
          }
        }

        if (connected)
        {
          try
          {
            outStream = btSocket.getOutputStream();
            inStream = btSocket.getInputStream();
            Log.v(TAG, "Streams obtained");
          } catch (IOException e)
          {
            Log.v(TAG, "Stream error " + e.getMessage());
          }
        }
      }
    }
  }

  public void close() {
    try
    {
      btSocket.close();
      Log.v(TAG,"Socket closed");
    } catch (IOException e)
    {
      Log.v(TAG,"Close error " + e.getMessage());
    }
  }

  public void writeMessage(int length, byte[] data)
  {
    if (length>0)
    {
      try
      {
        outStream.write(data,0,length);
        Log.v(TAG,"Bytes written: " + length);
      } catch (IOException e)
      {
        Log.v(TAG,"Write error " + e.getMessage());
      }
    }
  }

  public void readMessage(int length, byte[] data,int offset)
  {
    int bytesRead = 0;
    try
    {
      bytesRead = inStream.read(data, offset, length);
      Log.v(TAG,"Bytes read: " + bytesRead);
    } catch (IOException e)
    {
      Log.v(TAG,"Read error " + e.getMessage());
    }
  }
}
