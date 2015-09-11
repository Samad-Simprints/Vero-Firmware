package com.simprints.scanner.library;

import android.util.Log;

/**
 * Created by derek on 18/08/2015.
 */
public class Scanner
{
  private Connection connection;

  public Scanner(Connection connection) {
    this.connection = connection;
  }

  public Template getTemplate() {
    Template template = new Template();

    connection.read(template.getData());
    Log.d("Scanner.read","template " + template.getData()[0] + template.getData()[1] + template.getData()[2]);

    // diagnostic only
    connection.write(template.getData());

    return template;
  }

  public int getBatteryPercent() {
    return 50;
  }
}
