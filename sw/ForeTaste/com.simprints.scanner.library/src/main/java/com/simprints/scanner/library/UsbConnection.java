package com.simprints.scanner.library;

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

  public void close()
  {
  }

  public void writeMessage(int length, byte[] data)
  {
  }

  public void readMessage(int length, byte[] data, int offset)
  {
  }
}
