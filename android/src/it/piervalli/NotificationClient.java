
package it.piervalli;

import org.qtproject.qt5.android.QtNative;

import java.lang.String;
import android.content.ContentValues;
import android.content.Context;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Vibrator;
import android.util.Log;
import android.content.Intent;
import android.content.IntentFilter;
import android.provider.Settings;
import android.view.View;
import java.io.File;
import java.io.FileOutputStream;
import android.net.Uri;
import android.R;
import android.net.wifi.WifiManager;
import android.os.BatteryManager;
import android.os.Build;
public class NotificationClient extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;
    private static NotificationClient m_instance;
    private static Vibrator m_vibrate;
    private static final String TAG = "Qt";
    private static  WifiManager m_WifiManager;
    public static String[] MY_PERMISSIONS_STORAGE = {
                "android.permission.READ_EXTERNAL_STORAGE",
                "android.permission.WRITE_EXTERNAL_STORAGE",
                "android.permission.MOUNT_UNMOUNT_FILESYSTEMS"};
    public NotificationClient()
    {
        m_instance = this;
        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }
    public static void vibrate(int s) {
        if(m_vibrate == null) {
                m_vibrate = (Vibrator)m_instance.getSystemService(Context.VIBRATOR_SERVICE);
            }

        if (m_vibrate.hasVibrator()) {
            Log.v("Can Vibrate", "YES");
            m_vibrate.vibrate(s);
        } else {
            Log.v("Can Vibrate", "NO");
        }

    }
    public static String nativeLibraryDir(){
        Log.v(m_instance.TAG,"nativeLibraryDir"+m_instance.getApplicationContext().getApplicationInfo().nativeLibraryDir);
        return m_instance.getApplicationContext().getApplicationInfo().nativeLibraryDir;
    }
    public static String directoryDownLoad() {
        Context context = m_instance.getApplicationContext();
        File dir = null;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R)
        {
            dir = android.os.Environment.getExternalStoragePublicDirectory(android.os.Environment.DIRECTORY_DOCUMENTS);
        }
        else
        {
             dir = context.getExternalFilesDir(android.os.Environment.DIRECTORY_DOWNLOADS);
        }

        // Make sure the path directory exists.
        if (!dir.exists())
        {
            // Make it, if it doesn't exit
            boolean success = dir.mkdirs();
            if (!success)
            {
                dir = null;
                return "";
            }
        }

        Log.v(m_instance.TAG,"directoryDownLoad"+dir.getAbsolutePath());
        return dir.getAbsolutePath();
    }
    public static int installApp(String appPackageName) {
       if (QtNative.activity() == null)
           return -1;
       Context context = m_instance.getApplicationContext();
        return -1;
   }
    public static int getBatteryPercentage() {

        Context context = m_instance.getApplicationContext();
        Log.v(m_instance.TAG, "VERSION.SDK_INT "+Build.VERSION.SDK_INT);   // mostra sempre zero se viene abilitato

        IntentFilter iFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = context.registerReceiver(null, iFilter);

        int level = batteryStatus != null ? batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1) : -1;
        int scale = batteryStatus != null ? batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1) : -1;
        Log.v(m_instance.TAG, "BatteryPercentage level"+level);
        Log.v(m_instance.TAG, "BatteryPercentage scale "+scale);
        double batteryPct = level / (double) scale;

        return (int) (batteryPct * 100);
    }
    public static void notify(String s)
    {
        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
            m_builder = new Notification.Builder(m_instance);
            m_builder.setContentTitle("A message from Qt!");
        }

        m_builder.setContentText(s);
        m_notificationManager.notify(1, m_builder.build());
    }
    public static Intent openurl(String urlApk)
    {
       Intent m_view = new Intent(Intent.ACTION_VIEW);
       if(m_view==null){
        return null;
       }
       m_view.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
       m_view.setData(Uri.parse(urlApk));
       return m_view;
    }
    public static void restartWifi(int s) {
        if(m_WifiManager == null) {
            m_WifiManager = (WifiManager) m_instance.getSystemService(Context.WIFI_SERVICE);
        }
        // If wifi is on then off it
        m_WifiManager.setWifiEnabled(false);
        m_WifiManager.setWifiEnabled(true);
    }
	public static boolean setLocation(String aEnable) {
    boolean enable = (aEnable.equals("1") ? true : false);
    Log.v("setLocation",aEnable);
		return true;
  }

  public String getExternalStorage() {
      return "Todo";//m_instance.getApplicationContext().getExternalStorageDirectory();
  }
  public static String getDocumentsLocation() {

    File appFilesDirectory = m_instance.getApplicationContext().getFilesDir();
    File file = new File(m_instance.getApplicationContext().getFilesDir(), "pier_filename.txt");
    try {
      if (file.createNewFile()) {
        System.out.println("File created: " + file.getName());
      } else {
        System.out.println("File already exists.");
      }
    } catch (Exception e) {
      System.out.println("An error occurred.");
      e.printStackTrace();
    }
    return appFilesDirectory.getAbsolutePath();
  }

@Override
   protected void onStart() {
       super.onStart();
   }

 @Override
 protected void onDestroy() {
     super.onDestroy();
 }
}
