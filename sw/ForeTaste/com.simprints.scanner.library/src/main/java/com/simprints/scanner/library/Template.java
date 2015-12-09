package com.simprints.scanner.library;

/**
 * Created by derek on 18/08/2015.
 */
public class Template
{
  private int length;
  private byte[] data;

  public Template() {
    length = 0;
    data = new byte[400];
  }

  public void setData(int length, byte[] data)
  {
    this.length = length;
    this.data = data;
  }

  public int getLength()
  {
    return length;
  }

  public byte[] getData()
  {
    return data;
  }
}
