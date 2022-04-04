using UnityEditor;
using UnityEngine;

[ExecuteInEditMode]
public class Mirroring : MonoBehaviour
{
    Mesh mesh;
    Material material;

    private void OnEnable()
    {
        UnityEditor.SceneView.onSceneGUIDelegate -= OnSceneGUI;
        UnityEditor.SceneView.onSceneGUIDelegate += OnSceneGUI;
        mesh = GetComponent<MeshFilter>().sharedMesh;
        material = GetComponent<MeshRenderer>().sharedMaterial;
    }

    private void OnDisable()
    {
        UnityEditor.SceneView.onSceneGUIDelegate -= OnSceneGUI;
    }

    private void OnSceneGUI(SceneView sceneView)
    {
        Vector3 p = gameObject.transform.position;
        Quaternion q = gameObject.transform.rotation;
        Vector3 s = gameObject.transform.localScale;
        //Draw(sceneView.camera, gameObject.transform.localToWorldMatrix * Matrix4x4.TRS(new Vector3(-p.x * 0.5f, 0f, 0f), Quaternion.identity, Vector3.one));
        Draw(sceneView.camera, Matrix4x4.TRS(new Vector3(-p.x, p.y, p.z), q, s));
    }

    private void Update()
    {
        Draw(Camera.main, Matrix4x4.identity);
    }

    private void Draw(Camera camera, Matrix4x4 matrix)
    {
        if (mesh && material && camera)
        {
            Graphics.DrawMesh(mesh, matrix, material, gameObject.layer, camera);
        }
    }
}
