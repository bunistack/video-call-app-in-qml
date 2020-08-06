package com.bunistack.utils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;

public class PermissionsUtils {

    public static void requestPermission(Activity activity, Context context, String permission){

        //check if permission is not granted
        if(ContextCompat.checkSelfPermission(context,permission) == PackageManager.PERMISSION_DENIED){
            Log.d("tag", "granting permission");
            ActivityCompat.requestPermissions(activity, new String[]{permission}, 0);
        }else {
            Log.d("tag", "permission granted");
        }
    }
}
