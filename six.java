import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
public class six {

    public class Input {

        private static Long time;
        private static Long distance;

        private static void set_times(String input){
            var data = input.replace(" ", "");
            time = Long.valueOf(data);
        }

        private static void set_distances(String input){
            var data = input.replace(" ", "");
            distance = Long.valueOf(data);
        }

        private static int calculate_solution(){
            System.out.printf("Calculating winner for time %d over distance %d\n", time, distance);
            var ways_to_win = 0;
            // at start, hold button for x seconds => accelerates to x m/s
            for (int j = 0; j < time; j ++){
                var remaining_time = time - j;
                var final_time = remaining_time * j;
                if (final_time > distance){
                    ways_to_win ++;
                }
            }
            return ways_to_win;
        }

        public static void parse_file(String input_file){
            // File reading code copied from Stack Overflow:
            // https://stackoverflow.com/questions/45826412/how-to-parse-a-simple-text-file-in-java
            FileInputStream stream = null;
            try {
                stream = new FileInputStream(input_file);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            BufferedReader reader = new BufferedReader(new InputStreamReader(stream));
            String strLine;
            try {
                while ((strLine = reader.readLine()) != null) {
                    if (strLine.startsWith("Time:")){
                        var data = strLine.split(": ")[1];
                        set_times(data);
                    } else if (strLine.startsWith("Distance:")) {
                        var data = strLine.split(": ")[1];
                        set_distances(data);
                    } else {
                        throw new Exception(String.format(
                            "Unexpected input format. Incorrect line: %s\n", strLine));
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                reader.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return;
        }
    };


    public static void main(String args[]) {

        Input.parse_file(args[0]);
        System.out.printf("Solution: %d\n", Input.calculate_solution());
    }

}
