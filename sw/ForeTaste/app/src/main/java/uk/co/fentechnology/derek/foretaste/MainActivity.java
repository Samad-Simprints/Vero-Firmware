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
import com.simprints.scanner.library.Scanner;
import com.simprints.scanner.library.Template;
import com.simprints.scanner.library.Image;

public class MainActivity extends AppCompatActivity {
  private Connector connector = Connector.getInstance();
  private static Connection connection = null;
  private static int connectionIndex;
  private static Scanner scanner;
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

  // button handler - auto select connection
  public void selectAutoConnection(View view)
  {
    if (connection == null)
    {
      connection = connector.autoConnect();
    }
    viewHolder.update();
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
      scanner = null;
    }
    viewHolder.update();
  }

  // button handler - connect/clear scanner
  public void connectScanner(View view) {
    if (scanner == null)
    {
      scanner = new Scanner(connection);
    }
    else
    {
      scanner = null;
    }
    viewHolder.update();
  }

  // button handler - scanTemplateBtn
  public void scanTemplate(View view) {
    Template template = scanner.getTemplate();
    viewHolder.showTemplate(template.getData());
  }

  // button handler - scanImageBtn
  public void scanImage(View view) {
    Image image = scanner.getImage();
    viewHolder.showImage(image.getData());
  }

  // button handler - battery
  public void battery(View view) {
    viewHolder.showBattery(scanner.getBatteryPercent());
  }

  // local class to handle view and updates
  class ViewHolder {
    private TextView connectNameTxt;
    private Button selectScannerBtn;
    private TextView scannerNameTxt;
    private Button selectAutoConnectionBtn;
    private Button selectConnectionBtn;
    private Button scanTemplateBtn;
    private Button scanImageBtn;
    private Button batteryBtn;
    private TextView templateTxt;

    public ViewHolder(Activity activity)
    {
      connectNameTxt = (TextView) activity.findViewById(R.id.connectName);
      selectAutoConnectionBtn = (Button) activity.findViewById(R.id.connectAutoBtn);
      selectConnectionBtn = (Button) activity.findViewById(R.id.connectBtn);
      scannerNameTxt = (TextView) activity.findViewById(R.id.scannerName);
      selectScannerBtn = (Button) activity.findViewById(R.id.scannerBtn);
      scanTemplateBtn = (Button) activity.findViewById(R.id.scanTemplateBtn);
      scanImageBtn = (Button) activity.findViewById(R.id.scanImageBtn);
      batteryBtn = (Button) activity.findViewById(R.id.batteryBtn);
      templateTxt = (TextView) activity.findViewById(R.id.templateTxt);
    }

    public void setConnectName() {
      connectNameTxt.setText((connection == null) ? "No Connection" : connection.name());
    }

    public void setScannerName() {
      scannerNameTxt.setText((scanner == null) ? "None": connection.deviceName());
    }

    public void update()
    {
      if (connection==null)
      {
        selectConnectionBtn.setText("Select");
        selectAutoConnectionBtn.setEnabled(true);
        selectScannerBtn.setEnabled(false);
      }
      else
      {
        selectConnectionBtn.setText("Clear");
        selectAutoConnectionBtn.setEnabled(false);
        selectScannerBtn.setEnabled(true);
      }

      if (scanner==null)
      {
        scanTemplateBtn.setEnabled(false);
        scanImageBtn.setEnabled(false);
        batteryBtn.setEnabled(false);
        selectScannerBtn.setText("Connect");
      }
      else
      {
        scanTemplateBtn.setEnabled(true);
        scanImageBtn.setEnabled(true);
        batteryBtn.setEnabled(true);
        selectScannerBtn.setText("Clear");
      }

      setConnectName();
      setScannerName();
    }

    public void showTemplate(byte[] r) {
      String str = "Template: ";

      for ( byte b : r )
      {
        str = str.concat(String.valueOf((char)b));
      }

      templateTxt.setText(str);
    }

    public void showImage(byte[] r) {
      String str = "Image: ";

      for ( byte b : r )
      {
        str = str.concat(String.valueOf((char)b));
      }

      templateTxt.setText(str);
    }

    public void showBattery(int b) {
      String str = "Battery: " + b +"%";
      templateTxt.setText(str);
    }
  }

}
