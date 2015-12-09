package com.simprints.scanner.library;

import android.app.Activity;

/**
 * Created by derek on 25/08/2015.
 */
public class UsbConnection extends Connection
{
  public UsbConnection()
  {
    super("USB");
  }

  public boolean init() {
    errorMessage = "Not yet implemented";
    isSetup = false;
    return false;
  }

  public String deviceName()
  {
    return "USB Dev";
  }

  public String deviceDetail() {
    return "USB Dev Details";
  }

  public void open() {

  }

  public void close() {

  }

  public void readResponse(int length, byte[] data)
  {
  }

  public void writeCommand(byte cmd, int length, byte[] data)
  {
  }
}
