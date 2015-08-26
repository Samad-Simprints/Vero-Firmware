package uk.co.fentechnology.derek.foretaste;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Layout;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;

public class MainActivity extends AppCompatActivity {
  private Connector connector = Connector.getInstance();
  private static Connection connection = null;
  private TextView connectName;
  private TextView scannerName;
  private Button selectScanner;
  private Button selectConnection;
  private View scannerLayout;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    connectName = (TextView) this.findViewById(R.id.connectName);
    selectConnection = (Button) this.findViewById(R.id.connectBtn);
    scannerName = (TextView) this.findViewById(R.id.scannerName);
    selectScanner = (Button) this.findViewById(R.id.scannerBtn);

    setName();
    selectScanner.setEnabled(connection != null);

    Log.i("MainActivity", "onCreate");
  }

  // request values
  static final int PICK_CONNECTION = 1;
  static final int PICK_SCANNER    = 2;

  @Override
  protected void onActivityResult(int requestCode,int resultCode,Intent data) {
    // check which request this was
    if (resultCode==RESULT_OK)
    {
      switch (requestCode)
      {
        case PICK_CONNECTION:
          int selection = data.getIntExtra("selection",0);
          Connection c = connector.get(selection);
          if (c.isSetup())
          {
            connection = c;
            setName();
            selectScanner.setEnabled(true);
            selectConnection.setText("Clear");
          }
          else
          {
            Toast.makeText(this,c.name() + " is not ready",Toast.LENGTH_SHORT).show();
          }
          break;

        case PICK_SCANNER:
          break;
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

  public void openSearch() {
    Log.i("openSearch", "called");
  }

  public void openSettings() {
    Intent intent = new Intent(this, ConnectionActivity.class);
    startActivityForResult(intent, PICK_CONNECTION);
  }

  public void selectConnection(View view)
  {
    if (connection == null)
    {
      openSettings();
    }
    else
    {
      connection = null;
      selectConnection.setText("Select");
    }
    setName();
  }

  public void selectScanner(View view) {
    int[] devices = connection.deviceList();

    if (devices.length>0)
    {
      scannerName.setText(connection.deviceName(0));
    }
    // Intent intent = new Intent(this, S)

  }

  private void setName() {
    connectName.setText((connection==null)? "No Connection": connection.name());  }
}
