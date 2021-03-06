#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>

int main() {
  constexpr size_t num_verts = 8;
  constexpr float verts_1[] = {
      -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f,
      -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f, +1.0f,
      -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f,
  };
  constexpr float verts_2[] = {
      -2.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f,
      -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f, +1.0f,
      -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +2.0f, +1.0f, +1.0f,
  };
  constexpr float verts_3[] = {
      -2.0f, -1.0f, -1.0f, +1.0f, -1.0f, -2.0f, -1.0f, +1.0f,
      -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f, +1.0f,
      -1.0f, +1.0f, -1.0f, +1.0f, +2.0f, +2.0f, +1.0f, +1.0f,
  };
  constexpr size_t num_indices = 24;
  constexpr int32_t indices[] = {
      0, 4, 6, 2, 5, 1, 3, 7, 0, 1, 5, 4, 6, 7, 3, 2, 1, 0, 2, 3, 4, 5, 7, 6,
  };
  constexpr size_t num_counts = 6;
  constexpr int32_t counts[] = {4, 4, 4, 4, 4, 4};
  constexpr float dt = 1.0f / 60.0f;

  Alembic::Abc::OArchive archive(Alembic::AbcCoreOgawa::WriteArchive(),
                                 "/home/yohar/Desktop/vertex-animation.abc");

  const Alembic::Abc::TimeSampling time_sampling(dt, 0);
  const uint32_t time_sampling_index = archive.addTimeSampling(time_sampling);

  Alembic::AbcGeom::OPolyMesh mesh_obj(
      Alembic::Abc::OObject(archive, Alembic::Abc::kTop), "mesh");
  Alembic::AbcGeom::OPolyMeshSchema& mesh = mesh_obj.getSchema();

  mesh.setTimeSampling(time_sampling_index);

  const Alembic::AbcGeom::OPolyMeshSchema::Sample mesh_samp_1(
      Alembic::Abc::V3fArraySample((const Alembic::Abc::V3f*)verts_1,
                                   num_verts),
      Alembic::Abc::Int32ArraySample(indices, num_indices),
      Alembic::Abc::Int32ArraySample(counts, num_counts));
  const Alembic::AbcGeom::OPolyMeshSchema::Sample mesh_samp_2(
      Alembic::Abc::V3fArraySample((const Alembic::Abc::V3f*)verts_2,
                                   num_verts));
  const Alembic::AbcGeom::OPolyMeshSchema::Sample mesh_samp_3(
      Alembic::Abc::V3fArraySample((const Alembic::Abc::V3f*)verts_3,
                                   num_verts));

  mesh.set(mesh_samp_1);
  mesh.set(mesh_samp_2);
  mesh.set(mesh_samp_3);

  return 0;
}
