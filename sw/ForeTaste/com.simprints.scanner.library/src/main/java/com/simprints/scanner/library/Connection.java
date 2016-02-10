package com.simprints.scanner.library;

import android.app.Activity;
import android.app.IntentService;
import android.content.Intent;

import java.util.ArrayList;

/**
 * Created by derek on 19/08/2015.
 */
public abstract class Connection
{
  private String name;
  private ConnectionCallback callback;
  private Intent mServiceIntent = null;

  protected boolean isSetup;
  protected String errorMessage = "No Error";

  public Connection(String name) {
    this.name = name;
    this.isSetup = false;
//    this.mServiceIntent = new Intent(this.getActivity(),ConnectionListener.class);
//    this.mServiceIntent.setData()
  }

  public String name() { return this.name; }

  public boolean isSetup() { return this.isSetup; }

  public String getErrorMessage() {
    return errorMessage;
  }

  public boolean open(ConnectionCallback callback)
  {
    this.callback = callback;
    return true;
  };

  public void readMessage(int length, byte[] data)
  {
    readMessage(length,data,0);
  }

  abstract public boolean init();
  abstract public String deviceName();
  abstract public String deviceDetail();
  abstract public void close();
  abstract public void writeMessage(int length, byte[] data);
  abstract public void readMessage(int length, byte[] data,int offset);
}

