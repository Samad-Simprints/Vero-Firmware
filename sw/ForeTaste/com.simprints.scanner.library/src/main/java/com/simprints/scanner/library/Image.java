package com.simprints.scanner.library;

/**
 * Created by derek on 18/08/2015.
 */
public class Image
{
  private int length;
  private boolean compressed;
  private byte[] data;

  public Image() {
    length = 12000;
    compressed = false;
    data = new byte[12000];
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
