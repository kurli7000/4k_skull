using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PrintObjects : MonoBehaviour
{
    public string Name = "ballz";
    public bool Glsl = true;

    void Start ()
	{
		int cnt = 0;
		GameObject[] allObjects = UnityEngine.Object.FindObjectsOfType<GameObject>() ;

		string str = string.Empty;
		             
		foreach (GameObject go in allObjects)
		{
			if (go.activeInHierarchy)
			{
				if (go.GetComponent<MeshFilter>() == null)
					continue;

                float s = go.transform.localScale.x / 2f;

                if (Glsl)
                {
                    str += "vec4(" + go.transform.position.x.ToString("F3") + ","
                        + go.transform.position.y.ToString("F3") + ","
                        + go.transform.position.z.ToString("F3") + ","
                        + s.ToString("F3") + "),\n";
                }
                else
                {
                    str += go.transform.position.x.ToString("F3") + "f,"
                        + go.transform.position.y.ToString("F3") + "f,"
                        + go.transform.position.z.ToString("F3") + "f,"
                        + s.ToString("F3") + "f,\n";
                }

				cnt++;
			}
		}

        if (Glsl)
        {
            str += ");\n";
            Debug.Log("const int " + Name + "_cnt = " + cnt + ";\nvec4 " + Name + "[" + Name + "_cnt] = vec4[](\n" + str);
        }
        else
        {
            str += "};\n";
            Debug.Log("const int " + Name + "_cnt = " + cnt * 4 + ";\nfloat " + Name + "[" + Name + "_cnt] = {\n" + str);
        }
    }
}


