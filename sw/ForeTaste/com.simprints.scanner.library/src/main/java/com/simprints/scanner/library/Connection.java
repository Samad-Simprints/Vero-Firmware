package com.simprints.scanner.library;

/**
 * Created by derek on 19/08/2015.
 */
public class Connection
{
  private int method;
  private String name;
  private boolean isActive;
  private boolean isSetup;

  Connection(int method, String name) {
    this.method = method;
    this.name = name;
    this.isActive = false;
    this.isSetup = false;
  }

  public String name() { return this.name; }

  public boolean isSetup() { return this.isSetup; }
}
