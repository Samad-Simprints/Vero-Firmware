package uk.co.fentechnology.derek.foretaste;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.simprints.scanner.library.Connection;
import com.simprints.scanner.library.Connector;

public class MainActivity extends AppCompatActivity {
  private Connector connector = Connector.getInstance();
  private static Connection connection = null;
  private static int connectionIndex;
  private static int scannerIndex = -1;
  private ViewHolder viewHolder;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    viewHolder = new ViewHolder(this);
    viewHolder.update();

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
            connectionIndex = selection;
          }
          else
          {
            Toast.makeText(this,c.name() + " is not ready",Toast.LENGTH_SHORT).show();
          }
          break;

        case PICK_SCANNER:
          scannerIndex = data.getIntExtra("scanner",0);
          break;
      }
      viewHolder.update();
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

  // button handler - select/clear connection
  public void selectConnection(View view)
  {
    if (connection == null)
    {
      openSettings();
    }
    else
    {
      connection = null;
      scannerIndex = -1; // clearing connection also clears scanner
    }
    viewHolder.update();
  }

  // button handler - select/clear scanner
  public void selectScanner(View view) {
    if (scannerIndex<0)
    {
      int[] devices = connection.deviceList();

      if (devices.length==0)
      {
        Toast.makeText(this,"No Scanners Found",Toast.LENGTH_SHORT).show();
      }
      else if (devices.length==1)
      {
        scannerIndex = 1;
      }
      else
      {
        Intent intent = new Intent(this, SelectionActivity.class);
        intent.putExtra("index",connectionIndex);
        startActivityForResult(intent, PICK_SCANNER);
      }
    }
    else
    {
      scannerIndex = -1;
    }
    viewHolder.update();
  }

  class ViewHolder {
    private TextView connectNameTxt;
    private Button selectScannerBtn;
    private TextView scannerNameTxt;
    private Button selectConnectionBtn;

    public ViewHolder(Activity activity)
    {
      connectNameTxt = (TextView) activity.findViewById(R.id.connectName);
      selectConnectionBtn = (Button) activity.findViewById(R.id.connectBtn);
      scannerNameTxt = (TextView) activity.findViewById(R.id.scannerName);
      selectScannerBtn = (Button) activity.findViewById(R.id.scannerBtn);
    }

    public void setConnectName() {
      connectNameTxt.setText((connection == null) ? "No Connection" : connection.name());
    }

    public void setScannerName() {
      scannerNameTxt.setText((scannerIndex<0) ? "None": connection.deviceName(scannerIndex));
    }

    public void update()
    {
      if (connection==null)
      {
        selectConnectionBtn.setText("Select");
        selectScannerBtn.setEnabled(false);
      }
      else
      {
        selectConnectionBtn.setText("Clear");
        selectScannerBtn.setEnabled(true);
      }

      selectScannerBtn.setText((scannerIndex<0)? "Select": "Clear");

      setConnectName();
      setScannerName();
    }
  }

}
