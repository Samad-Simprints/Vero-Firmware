package com.simprints.scanner.library;

import android.app.Activity;

import java.util.ArrayList;

/**
 * Created by derek on 19/08/2015.
 */
public abstract class Connection
{
  private String name;
  private boolean isActive;
  protected boolean isSetup;
  protected String errorMessage = "No Error";

  public Connection(String name) {
    this.name = name;
    this.isActive = false;
    this.isSetup = false;
  }

  public String name() { return this.name; }

  public boolean isSetup() { return this.isSetup; }

  public String getErrorMessage() {
    return errorMessage;
  }

  abstract public boolean init(Activity activity);
  abstract public int[] deviceList();
  abstract public String deviceName(int index);
  abstract public void open(int index);
  abstract public void close();
  abstract public void read();
}
