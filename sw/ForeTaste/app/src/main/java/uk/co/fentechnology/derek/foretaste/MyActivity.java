package uk.co.fentechnology.derek.foretaste;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.content.Intent;
import android.view.View;
import android.widget.EditText;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;
import com.simprints.scanner.library.Scanner;
import com.simprints.scanner.library.ScannerLib;

import java.util.ArrayList;

import javax.sql.ConnectionEvent;

public class MyActivity extends AppCompatActivity {
  public final static String EXTRA_MESSAGE = "uk.co.fentechnology.derek.foretaste.MESSAGE";
  private ScannerLib scannerLib = ScannerLib.getInstance();

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_my);
  }

  // request values
  static final int PICK_CONNECTION = 1;

  @Override
  protected void onActivityResult(int requestCode,int resultCode,Intent data) {
    // check which request this was
    if (requestCode==PICK_CONNECTION)
    {
      if (resultCode==RESULT_OK)
      {
        int selection = data.getIntExtra("selection",0);
        Log.i("result",String.valueOf(selection));
        Connection connection = scannerLib.get(selection);
        if (!connection.isSetup())
        {
          connection.setup();
        }
      }
    }
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    // Inflate the menu; this adds items to the action bar if it is present.
    getMenuInflater().inflate(R.menu.menu_my, menu);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    // Handle action bar item clicks here. The action bar will
    // automatically handle clicks on the Home/Up button, so long
    // as you specify a parent activity in AndroidManifest.xml.
    switch (item.getItemId())
    {
      case R.id.action_search:
        openSearch();
        return true;

      case R.id.action_settings:
        openSettings();
        return true;

      default:
        return super.onOptionsItemSelected(item);
    }
  }

  /* called when the user clicks the Send button */
  public void sendMessage(View view) {
    Intent intent = new Intent(this, DisplayMessageActivity.class);
    EditText editText = (EditText)findViewById(R.id.edit_message);
    String message = editText.getText().toString();
    intent.putExtra(EXTRA_MESSAGE, message);
    startActivity(intent);
  }

  public void openSearch()
  {
    Log.i("openSearch", "called");
  }

  public void openSettings()
  {
    Intent intent = new Intent(this, ConnectionActivity.class);
    startActivityForResult(intent, PICK_CONNECTION);
  }
}
