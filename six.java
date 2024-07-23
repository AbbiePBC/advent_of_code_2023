import java.util.ArrayList;
import java.util.List;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
public class six {

    public class Input {

        private static List<Integer> times = new ArrayList<Integer>();
        private static List<Integer> distances = new ArrayList<Integer>();

        private static void set_times(String input){
            var data = input.split(" ");
            for (var val: data){
                if (!val.equals("")){
                    times.add(Integer.valueOf(val));
                }
            }
        }

        private static void set_distances(String input){
            var data = input.split(" ");
            for (var val: data){
                if (!val.equals("")){
                    distances.add(Integer.valueOf(val));
                }
            }
        }

        private static int ways_to_win_race(int i){
            var time = times.get(i);
            var dist = distances.get(i);
            var ways_to_win = 0;
            // at start, hold button for x seconds => accelerates to x m/s
            for (int j = 0; j < time; j ++){
                var remaining_time = time - j;
                var final_time = remaining_time * j;
                if (final_time > dist){
                    ways_to_win ++;
                }
            }
            return ways_to_win;
        }

        public static int calculate_solution(){
            var sol = 1;
            for (int i = 0; i < times.size(); i ++){
                System.out.printf("Ways to win race %d = %d\n", i, ways_to_win_race(i));
                sol *= ways_to_win_race(i);
            }
            return sol;
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
                            "Unexpected input format. Incorrect line: %s", strLine));
                    }
                }
                System.out.println(times);
                System.out.println(distances);
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
