package com.simprints.scanner.library;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.util.Log;

import java.util.Set;

import static android.support.v4.app.ActivityCompat.startActivityForResult;

/**
 * Created by derek on 25/08/2015.
 */
public class BtConnection extends Connection
{
  public BtConnection()
  {
    super("Bluetooth Classic");
  }

  public void enable(Activity activity) {
    // ask user to enable BT, then try again later ..
    int REQUEST_ENABLE_BT = 1;
    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
    startActivityForResult(activity,enableBtIntent,REQUEST_ENABLE_BT,null);
  }

  public boolean init() {
    boolean result = false;

    BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    if (mBluetoothAdapter == null)
    {
      errorMessage = "No Bluetooth found";
      isSetup = false;
    }
    else if (!mBluetoothAdapter.isEnabled())
    {
      errorMessage = "Bluetooth disabled";
      isSetup = false;
    }
    else
    {
      Set<BluetoothDevice> bluetoothDevices;
      // look for device
      bluetoothDevices = mBluetoothAdapter.getBondedDevices();
      if (bluetoothDevices.size()>0)
      {
        Log.v("BtConnection","BT devices found");
      }
    }

    return result;
  }

  public int[] deviceList() {
    return new int[0];
  }

  public String deviceName(int index)
  {
    return "BT Dev" + String.valueOf(index);
  }

  public String deviceDetail(int index) {
    return "BT Dev" + String.valueOf(index) + " Details";
  }

  public void open(int index) {

  }

  public void close() {

  }

  public void read(byte[] r) {

  }

  public void write(byte[] w) {

  }
}
