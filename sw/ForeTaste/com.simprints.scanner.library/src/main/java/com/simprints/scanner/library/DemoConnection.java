package com.simprints.scanner.library;

import android.app.Activity;
import android.util.Log;

import java.util.Random;

/**
 * Created by derek on 24/08/2015.
 */
public class DemoConnection extends Connection
{
  public DemoConnection()
  {
    super("Demonstration");
  }

  public boolean init() {
    isSetup = true;
    return true;
  }

  public String deviceName()
  {
    return "Demo Dev";
  }

  public String deviceDetail() {
    return "Demo Device Details";
  }

  public void open() {

  }

  public void close() {

  }

  public void writeCommand(byte cmd, int length, byte[] data)
  {
    int i;

    switch(cmd)
    {
      case Scanner.CMD_IMAGE:
        break;

      case Scanner.CMD_TEMPLATE:
        break;

      case Scanner.CMD_GET_PARAMS:
        break;

      case Scanner.CMD_SET_PARAMS:
        break;

      case Scanner.CMD_STORE_IMAGE:
        break;

      case Scanner.CMD_FETCH_IMAGE:
        break;

      case Scanner.CMD_BATTERY:
        break;

      case Scanner.CMD_LEDS:
        break;
    }
  }

  public void readResponse(int length, byte[] data)
  {
    int i;

    for ( i = 0; i<length; ++i )
    {
      data[i] = 0;
    }
  }

}
