package com.simprints.scanner.library;

import android.app.Activity;

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
  abstract public void read();
  abstract public void open();
  abstract public void close();
}
