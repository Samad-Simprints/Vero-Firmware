package com.simprints.scanner.library;

import android.app.Activity;

/**
 * Created by derek on 24/08/2015.
 */
public class DemoConnection extends Connection
{
  public DemoConnection()
  {
    super("Demonstration");
  }

  public boolean init(Activity activity) {
    isSetup = true;
    return true;
  }

  public void read() {

  }

  public void open() {

  }

  public void close() {

  }
}
