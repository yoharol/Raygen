#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <raylib.h>
#include <Raygen/geometry.h>

int main() {
  std::vector<int> aQuad = {
      0, 4, 6, 2, 5, 1, 3, 7, 0, 1, 5, 4, 6, 7, 3, 2, 1, 0, 2, 3, 4, 5, 7, 6,
  };
  std::vector<float> aXYZ = {
      -1.f, -1.f, -1.f, +1.f, -1.f, -1.f, -1.f, +1.f, -1.f, +1.f, +1.f, -1.f,
      -1.f, -1.f, +1.f, +1.f, -1.f, +1.f, -1.f, +1.f, +1.f, +1.f, +1.f, +1.f,
  };
  const float dt = 1.0f / 60.0f;

  // Render the model
  //--------------------------------------------------------------------------------------

  // Raygen::gen_mesh(aXYZ.size(), aXYZ.data(), aQuad.size(), aQuad.data());

  std::string filepath = std::string(ASSETSPATH) + "blender/simple.abc";
  // Export to alembic
  //--------------------------------------------------------------------------------------

  Alembic::Abc::OArchive archive(Alembic::AbcCoreOgawa::WriteArchive(),
                                 (filepath).c_str());
  Alembic::AbcGeom::OPolyMesh mesh_obj(
      Alembic::Abc::OObject(archive, Alembic::Abc::kTop), "mesh");
  {  // set time sampling
    const Alembic::Abc::TimeSampling time_sampling(dt, 0);
    const uint32_t time_sampling_index = archive.addTimeSampling(time_sampling);
    mesh_obj.getSchema().setTimeSampling(time_sampling_index);
  }
  {  // set geometry
    std::vector<int> aElmSize(aQuad.size() / 4, 4);
    const Alembic::AbcGeom::OPolyMeshSchema::Sample mesh_samp(
        Alembic::AbcGeom::V3fArraySample((const Alembic::Abc::V3f *)aXYZ.data(),
                                         aXYZ.size() / 3),
        Alembic::AbcGeom::Int32ArraySample(aQuad.data(), aQuad.size()),
        Alembic::AbcGeom::Int32ArraySample(aElmSize.data(), aElmSize.size()));
    mesh_obj.getSchema().set(mesh_samp);
  }

  // import back from alembic and show in raylib
  //--------------------------------------------------------------------------------------
  Alembic::Abc::IArchive read_archive(Alembic::AbcCoreOgawa::ReadArchive(),
                                      filepath.c_str());
  Alembic::AbcGeom::IPolyMesh read_mesh(
      Alembic::Abc::IObject(read_archive, Alembic::Abc::kTop), "mesh");

  return 0;
}
