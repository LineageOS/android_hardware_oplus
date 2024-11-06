package android.bluetooth;

public class OplusBluetoothAdapter {

    private static OplusBluetoothAdapter sAdapter = null;
    
    public static OplusBluetoothAdapter getOplusBluetoothAdapter() {
        if (sAdapter == null) {
            sAdapter = new OplusBluetoothAdapter();
        }
        return sAdapter;
    }
}
