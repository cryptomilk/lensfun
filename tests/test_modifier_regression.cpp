#include <string>
#include <limits>
#include <map>
#include <cmath>

#include "lensfun.h"
#include "../libs/lensfun/lensfunprv.h"


typedef struct
{
  lfDatabase *db;
  size_t img_height;
  size_t img_width;
} lfFixture;

// setup a standard lens
void mod_setup (lfFixture *lfFix, gconstpointer data)
{

    lfFix->db = new lfDatabase ();
    lfFix->db->Load("data/db");

    lfFix->img_height = 1000;
    lfFix->img_width  = 1500;
}

void mod_teardown (lfFixture *lfFix, gconstpointer data)
{
    delete lfFix->db;
}

void test_verify_dist_poly3 (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "pEntax 50-200 ED");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "smc Pentax-DA 50-200mm f/4-5.6 DA ED");

    lfModifier* mod = new lfModifier (1.534f, lfFix->img_width, lfFix->img_height, LF_PF_F32, false);

    mod->EnableDistortionCorrection(lenses[0], 80.89f);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    float expected_x[] = {-14.03764153, 751.00000000, 810.27246094, 1275.17346191};
    float expected_y[] = {-9.35532570, 497.00000000, 938.97027588, 96.02919769};

    float coords [2];
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyGeometryDistortion (x[i], y[i], 1, 1, coords));
        //g_print("\n%.8f, %.8f\n", coords[0], coords[1]);
        g_assert_cmpfloat (fabs (coords [0] - expected_x [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [1] - expected_y [i]), <=, 1e-3);
    }

    delete mod;

    // manually create a lens object
    lfLensCalibDistortion lensCalibDist;
    lenses[0]->InterpolateDistortion(1.534f, 80.89f, lensCalibDist);
    lfLens* lens = new lfLens();
    lens->AddCalibDistortion(&lensCalibDist);
    lens->CropFactor = lenses[0]->CropFactor;
    lens->AspectRatio = lenses[0]->AspectRatio;

    mod = new lfModifier (1.534f, lfFix->img_width, lfFix->img_height, LF_PF_F32, false);
    mod->EnableDistortionCorrection(lens, 80.89f);

    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyGeometryDistortion (x[i], y[i], 1, 1, coords));
        //g_print("\n%.8f, %.8f\n", coords[0], coords[1]);
        g_assert_cmpfloat (fabs (coords [0] - expected_x [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [1] - expected_y [i]), <=, 1e-3);
    }

    delete mod;
    delete lens;

    lf_free (lenses);
}

void test_verify_dist_poly5 (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "Canon PowerShot G12");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "Canon PowerShot G12 & compatibles (Standard)");

    g_print("%s", lenses[0]->Model);

    lfModifier* mod = new lfModifier (4.6f, lfFix->img_width, lfFix->img_height, LF_PF_F32, false);

    mod->EnableDistortionCorrection(lenses[0], 10.89f);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    float expected_x[] = {28.85699272, 751.00000000, 809.50451660, 1260.12316895};
    float expected_y[] = {19.23155594, 497.00000000, 933.41711426, 107.58076477};

    float coords [2];
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyGeometryDistortion (x[i], y[i], 1, 1, coords));
        //g_print("\n%.8f, %.8f\n", coords[0], coords[1]);
        g_assert_cmpfloat (fabs (coords [0] - expected_x [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [1] - expected_y [i]), <=, 1e-3);
    }

    delete mod;
    lf_free (lenses);
}

void test_verify_dist_ptlens (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "PENTAX-F 28-80mm");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "Pentax-F 28-80mm f/3.5-4.5");

    lfModifier* mod = new lfModifier (1.534f, lfFix->img_width, lfFix->img_height, LF_PF_F32, false);

    mod->EnableDistortionCorrection(lenses[0], 30.89f);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    float expected_x[] = {29.04440117, 750.99969482, 808.74157715, 1255.12915039};
    float expected_y[] = {19.35648155, 497.00045776, 927.89971924, 111.41387939};

    float coords [2];
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyGeometryDistortion (x[i], y[i], 1, 1, coords));
        //g_print("\n%.8f, %.8f\n", coords[0], coords[1]);
        g_assert_cmpfloat (fabs (coords [0] - expected_x [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [1] - expected_y [i]), <=, 1e-3);
    }

    delete mod;
    lf_free (lenses);
}

void test_verify_vignetting_pa (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "Olympus ED 14-42mm");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "Olympus Zuiko Digital ED 14-42mm f/3.5-5.6");

    lfModifier* mod = new lfModifier (2.0f, lfFix->img_width, lfFix->img_height, LF_PF_U16, false);

    mod->EnableVignettingCorrection(lenses[0], 17.89f, 5.0f, 1000.0f);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    lf_u16 expected[] = {22422, 22422, 24174, 28848};

    lf_u16 coords [3] = {16000, 16000, 16000};
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyColorModification(&coords[0], x[i], y[i], 1, 1, LF_CR_3(RED,GREEN,BLUE), 0));
        //g_print("\n%d, %d, %d\n", coords[0], coords[1], coords[2]);
        g_assert_cmpfloat (fabs (coords [0] - expected [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [1] - expected [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [2] - expected [i]), <=, 1e-3);
    }

    delete mod;

    // manually create a lens object
    lfLensCalibVignetting lensCalibVign;
    lenses[0]->InterpolateVignetting(17.89f, 5.0f, 1000.0f, lensCalibVign);
    lfLens* lens = new lfLens();
    lens->AddCalibVignetting(&lensCalibVign);
    lens->CropFactor = lenses[0]->CropFactor;
    lens->AspectRatio = lenses[0]->AspectRatio;

    mod = new lfModifier (2.0f, lfFix->img_width, lfFix->img_height, LF_PF_U16, false);
    mod->EnableVignettingCorrection(lens, 17.89f, 5.0f, 1000.0f);

    coords[0] = 16000; coords[1] = 16000; coords[2] = 16000;
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyColorModification(&coords[0], x[i], y[i], 1, 1, LF_CR_3(RED,GREEN,BLUE), 0));
        //g_print("\n%d, %d, %d\n", coords[0], coords[1], coords[2]);
        g_assert_cmpfloat (fabs (coords [0] - expected [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [1] - expected [i]), <=, 1e-3);
        g_assert_cmpfloat (fabs (coords [2] - expected [i]), <=, 1e-3);
    }

    delete mod;
    delete lens;

    lf_free (lenses);
}

void test_verify_subpix_linear (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "Olympus ED 14-42mm");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "Olympus Zuiko Digital ED 14-42mm f/3.5-5.6");

    lfModifier* mod = new lfModifier (2.0f, lfFix->img_width, lfFix->img_height, LF_PF_U16, false);

    mod->EnableTCACorrection(lenses[0], 17.89f);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    float expected[][6] = {
        {-0.08681729, -0.05789410, 0.00002450, -0.00001032, -0.02400517, -0.01601936},
        {751.00061035, 496.99899292, 751.00000000, 497.00000000, 751.00000000, 497.00000000},
        {810.01995850, 937.14440918, 810.00000000, 937.00000000, 810.00042725, 937.00305176},
        {1270.12915039, 99.90086365, 1270.00000000, 100.00000763, 1270.00854492, 99.99343872}
    };

    float coords [6];
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplySubpixelDistortion(x[i], y[i], 1, 1, coords));
        //g_print("{%.8f, %.8f, %.8f, %.8f, %.8f, %.8f},\n", coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
        for (int j = 0; j < 6; j++)
            g_assert_cmpfloat (fabs (coords [j] - expected [i][j]), <=, 1e-3);
    }

    delete mod;
    lf_free (lenses);
}

void test_verify_subpix_poly3 (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "Olympus ED 14-42mm");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "Olympus Zuiko Digital ED 14-42mm f/3.5-5.6");

    lfModifier* mod = new lfModifier (2.0f, lfFix->img_width, lfFix->img_height, LF_PF_U16, false);

    mod->EnableTCACorrection(lenses[0], 26.89f);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    float expected[][6] = {
        {-0.05537901, -0.03692452, 0.00002450, -0.00001032, 0.01445518, 0.00962087},
        {751.00061035, 496.99902344, 751.00000000, 497.00000000, 750.99981689, 497.00030518},
        {810.01898193, 937.13732910, 810.00000000, 937.00000000, 809.99389648, 936.95599365},
        {1270.11572266, 99.91123199, 1270.00000000, 100.00000763, 1269.96374512, 100.02780914}
    };

    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        float coords [6];
        g_assert_true(mod->ApplySubpixelDistortion (x[i], y[i], 1, 1, coords));
        //g_print("{%.8f, %.8f, %.8f, %.8f, %.8f, %.8f},\n", coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
        for (int j = 0; j < 6; j++)
            g_assert_cmpfloat (fabs (coords [j] - expected [i][j]), <=, 1e-3);
    }

    delete mod;

    // manually create a lens object
    lfLensCalibTCA lensCalibTCA;
    lenses[0]->InterpolateTCA(26.89f, lensCalibTCA);
    lfLens* lens = new lfLens();
    lens->AddCalibTCA(&lensCalibTCA);
    lens->CropFactor = lenses[0]->CropFactor;
    lens->AspectRatio = lenses[0]->AspectRatio;

    mod = new lfModifier (2.0f, lfFix->img_width, lfFix->img_height, LF_PF_U16, false);
    mod->EnableTCACorrection(lens, 26.89f);

    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        float coords [6];
        g_assert_true(mod->ApplySubpixelDistortion (x[i], y[i], 1, 1, coords));
        //g_print("{%.8f, %.8f, %.8f, %.8f, %.8f, %.8f},\n", coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
        for (int j = 0; j < 6; j++)
            g_assert_cmpfloat (fabs (coords [j] - expected [i][j]), <=, 1e-3);
    }

    delete lens;
    delete mod;

    lf_free (lenses);
}


void test_verify_geom_fisheye_rectlinear (lfFixture *lfFix, gconstpointer data)
{
    // select a lens from database
    const lfLens** lenses = lfFix->db->FindLenses (NULL, NULL, "M.Zuiko Digital ED 8mm f/1.8 Fisheye");
    g_assert_nonnull(lenses);
    g_assert_cmpstr(lenses[0]->Model, ==, "Olympus M.Zuiko Digital ED 8mm f/1.8 Fisheye Pro");

    lfModifier* mod = new lfModifier (2.0f, lfFix->img_width, lfFix->img_height, LF_PF_U16, false);

    mod->EnableProjectionTransform(lenses[0], 8.0f, LF_RECTILINEAR);

    float x[] = {0, 751, 810, 1270};
    float y[] = {0, 497, 937, 100};

    float expected_x[] = {248.98896790, 751.00000000, 802.23010254, 1151.07922363};
    float expected_y[] = {165.93727112, 497.00000000, 880.81262207, 191.27542114};

    float coords [2];
    for (unsigned int i = 0; i < sizeof(x) / sizeof(float); i++)
    {
        g_assert_true(mod->ApplyGeometryDistortion (x[i], y[i], 1, 1, coords));
        //g_print("\n%.8f, %.8f\n", coords[0], coords[1]);
        g_assert_cmpfloat (fabs (coords [0] - expected_x [i]), <=, 1e-1);
        g_assert_cmpfloat (fabs (coords [1] - expected_y [i]), <=, 1e-1);
    }

    delete mod;
    lf_free (lenses);
}


int main (int argc, char **argv)
{
  setlocale (LC_ALL, "");
  setlocale(LC_NUMERIC, "C");

  g_test_init (&argc, &argv, NULL);

  g_test_add ("/modifier/coord/dist/verify_poly3", lfFixture, NULL,
              mod_setup, test_verify_dist_poly3, mod_teardown);

  g_test_add ("/modifier/coord/dist/verify_ptlens", lfFixture, NULL,
              mod_setup, test_verify_dist_ptlens, mod_teardown);

  g_test_add ("/modifier/coord/dist/verify_poly5", lfFixture, NULL,
              mod_setup, test_verify_dist_poly5, mod_teardown);

  g_test_add ("/modifier/coord/geom/verify_equisolid_linrect", lfFixture, NULL,
              mod_setup, test_verify_geom_fisheye_rectlinear, mod_teardown);

  g_test_add ("/modifier/color/vignetting/verify_pa", lfFixture, NULL,
              mod_setup, test_verify_vignetting_pa, mod_teardown);

  g_test_add ("/modifier/subpix/TCA/verify_linear", lfFixture, NULL,
              mod_setup, test_verify_subpix_linear, mod_teardown);

  g_test_add ("/modifier/subpix/TCA/verify_poly3", lfFixture, NULL,
              mod_setup, test_verify_subpix_poly3, mod_teardown);

  return g_test_run();
}
