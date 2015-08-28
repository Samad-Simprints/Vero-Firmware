package com.simprints.scanner.library;

import android.app.Activity;
import android.util.Log;

import java.util.Random;

/**
 * Created by derek on 24/08/2015.
 */
public class DemoConnection extends Connection
{
  private int[] devices = new int[2];
  public DemoConnection()
  {
    super("Demonstration");
  }

  public boolean init() {
    isSetup = true;
    return true;
  }

  public int[] deviceList() {
    return devices;
  }

  public String deviceName(int index)
  {
    return "Demo Dev" + String.valueOf(index);
  }

  public String deviceDetail(int index) {
    return "Demo Dev" + String.valueOf(index) + " Details";
  }

  public void open(int index) {

  }

  public void close() {

  }

  public void read(byte[] r) {
    Random rand = new Random();
    for ( int i = 0; i<r.length; ++i )
    {
      r[i] = (byte)(rand.nextInt(26) + (int)'a');
    }
    Log.d("DemoConnection","read " + r.length + " bytes");
  }

}
