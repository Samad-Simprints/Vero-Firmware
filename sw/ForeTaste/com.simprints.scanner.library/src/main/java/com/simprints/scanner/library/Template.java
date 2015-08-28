package com.simprints.scanner.library;

/**
 * Created by derek on 18/08/2015.
 */
public class Template
{
  private byte[] data;

  public Template() {
    data = new byte[400];
  }

  public void setData(byte[] data)
  {
    this.data = data;
  }

  public byte[] getData()
  {
    return data;
  }
}
