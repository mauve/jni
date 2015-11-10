
public class ObjectRefArray {
  public String[] getStringArray() {
    return new String[]{ "First", "Last" };
  }

  public void testStringArray(String[] input) throws Throwable {
    if (input.length != 2) {
      throw new Throwable("input.length != 2: " + input.length);
    }

    if (!input[0].equals("First")) {
      throw new Throwable("input[0] != First: '" + input[0] + "'");
    }

    if (!input[1].equals("NewString")) {
      throw new Throwable("input[1] != NewString: '" + input[1] + "'");
    }
  }
}