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

  public int[] deviceList() {
    return new int[0];
  }

  public String deviceName(int index)
  {
    return "USB Dev" + String.valueOf(index);
  }

  public String deviceDetail(int index) {
    return "USB Dev" + String.valueOf(index) + " Details";
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
