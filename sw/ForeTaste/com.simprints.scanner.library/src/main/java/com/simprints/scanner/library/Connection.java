package com.simprints.scanner.library;

/**
 * Created by derek on 19/08/2015.
 */
public abstract class Connection
{
  private String name;
  private boolean isActive;
  private boolean isSetup;

  public Connection(String name) {
    this.name = name;
    this.isActive = false;
    this.isSetup = false;
  }

  public String name() { return this.name; }

  public boolean isSetup() { return this.isSetup; }

  public void setup() {
    this.isSetup = true;
  }

  abstract void init();
  abstract void read();
  abstract void open();
  abstract void close();
}
