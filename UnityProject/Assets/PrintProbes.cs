using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

public class PrintProbes : MonoBehaviour
{
	void Start ()
	{
		SphericalHarmonicsL2 p = RenderSettings.ambientProbe;


		Debug.Log("const vec3 L00  = vec3(" + p[0, 0] + "," + p[1, 0] + "," + p[2, 0] + ");\n" +
		          "const vec3 L1m1 = vec3(" + p[0, 1] + "," + p[1, 1] + "," + p[2, 1] + ");\n" +
		          "const vec3 L10  = vec3(" + p[0, 2] + "," + p[1, 2] + "," + p[2, 2] + ");\n" +
		          "const vec3 L11  = vec3(" + p[0, 3] + "," + p[1, 3] + "," + p[2, 3] + ");\n" +
		          "const vec3 L2m2 = vec3(" + p[0, 4] + "," + p[1, 4] + "," + p[2, 4] + ");\n" +
		          "const vec3 L2m1 = vec3(" + p[0, 5] + "," + p[1, 5] + "," + p[2, 5] + ");\n" +
		          "const vec3 L20  = vec3(" + p[0, 6] + "," + p[1, 6] + "," + p[2, 6] + ");\n" +
		          "const vec3 L21  = vec3(" + p[0, 7] + "," + p[1, 7] + "," + p[2, 7] + ");\n" +
		          "const vec3 L22  = vec3(" + p[0, 8] + "," + p[1, 8] + "," + p[2, 8] + ");\n");
	}
	
}
