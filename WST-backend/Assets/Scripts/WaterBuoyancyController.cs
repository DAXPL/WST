using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering.HighDefinition;

public class OptimizedBoatPhysics : MonoBehaviour
{
    [Header("Settings")]
    public WaterSurface water;
    [Tooltip("U¿yj uproszczonego Mesha (np. Cube) dla wydajnoœci!")]
    public Mesh simplifiedHullMesh;
    public float fluidDensity = 1000f; // gêstoœæ wody
    public float displacementAmount = 2f; // Jak mocno wypiera wodê

    [Header("Hydrodynamics")]
    public float forwardDrag = 1f;  // Opór czo³owy
    public float sideDrag = 50f;    // Opór boczny (KIL) - to jest kluczowe!
    public float angularDrag = 2f;

    public Rigidbody rb;
    private Vector3[] voxels; // Nasze punkty wypornoœci
    private int voxelCount;

    void Awake()
    {

        // Jeœli nie poda³eœ uproszczonego mesha, weŸ ten z filtra (ryzykowne wydajnoœciowo!)
        if (simplifiedHullMesh == null) simplifiedHullMesh = GetComponent<MeshFilter>().mesh;

        voxels = simplifiedHullMesh.vertices;
        voxelCount = voxels.Length;

        if (water == null) water = (WaterSurface)FindFirstObjectByType(typeof(WaterSurface), FindObjectsInactive.Exclude);



        if (water == null)
        {

            Debug.LogError("No water surface found or attached.");
        }
    }
    void FixedUpdate()
    {
        if (water == null) return;

        float submergedPercent = 0f;
        int submergedCount = 0;

        // 1. Obliczanie Wypornoœci (Buoyancy) dla ka¿dego punktu
        for (int i = 0; i < voxelCount; i++)
        {
            Vector3 worldPt = transform.TransformPoint(voxels[i]);

            // Pobieramy parametry wody w danym punkcie (fale)
            WaterSearchParameters search = new WaterSearchParameters { startPositionWS = worldPt };
            WaterSearchResult result;

            if (water.ProjectPointOnWaterSurface(search, out result))
            {
                float waterHeight = result.projectedPositionWS.y;

                // Jeœli punkt jest pod wod¹
                if (worldPt.y < waterHeight)
                {
                    float depth = waterHeight - worldPt.y;

                    // Si³a wyporu Archimedesa (uproszczona)
                    // Dzielimy przez iloœæ punktów, ¿eby suma si³ by³a poprawna niezale¿nie od gêstoœci siatki
                    float buoyantForce = (fluidDensity * Mathf.Abs(Physics.gravity.y) * depth * displacementAmount) / voxelCount;

                    // Aplikujemy w punkcie - to daje nam stabilizacjê i przechy³y na falach
                    rb.AddForceAtPosition(Vector3.up * buoyantForce, worldPt, ForceMode.Force);

                    submergedCount++;
                }
            }
        }

        submergedPercent = (float)submergedCount / voxelCount;

        // 2. Aplikowanie Oporów (Hydrodynamics) - TYLKO jeœli ³ódŸ jest w wodzie
        if (submergedPercent > 0.1f)
        {
            ApplyHydrodynamics(submergedPercent);
        }
    }

    void ApplyHydrodynamics(float submersionFactor)
    {
        // Prêdkoœæ lokalna (wzglêdem dziobu ³odzi)
        Vector3 localVelocity = transform.InverseTransformDirection(rb.linearVelocity);

        // Kluczowe dla realizmu: Oddzielny opór dla przodu (Z) i boków (X)
        float dragZ = localVelocity.z * forwardDrag;
        float dragX = localVelocity.x * sideDrag; // To symuluje kil/miecz!
        float dragY = localVelocity.y * forwardDrag; // Opór przy zanurzaniu/wynurzaniu

        Vector3 localDragForce = new Vector3(-dragX, -dragY, -dragZ);

        // Transformujemy si³ê oporu z powrotem na œwiat i skalujemy przez zanurzenie
        Vector3 worldDragForce = transform.TransformDirection(localDragForce) * submersionFactor;

        rb.AddForce(worldDragForce, ForceMode.Force);

        // Opór obrotowy (te¿ zale¿ny od zanurzenia)
        rb.AddTorque(-rb.angularVelocity * angularDrag * submersionFactor, ForceMode.Force);
    }

    // Wizualizacja dla debugowania
    private void OnDrawGizmosSelected()
    {
        if (voxels == null) return;
        Gizmos.color = Color.cyan;
        foreach (var v in voxels)
        {
            Gizmos.DrawSphere(transform.TransformPoint(v), 0.05f);
        }
    }
}