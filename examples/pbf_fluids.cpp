#include <Raygen/Camera/orbit_camera.h>
#include <Raygen/Math/math.h>
#include <Raygen/Math/random.h>
#include <Raygen/draw.h>
#include <Raygen/file.h>
#include <Raygen/input.h>
#include <Raygen/particles.h>
#include <raylib.h>
#include <unordered_map>
#include <vector>

#define MAX_BATCH_ELEMENTS 8192

using VEC3   = Eigen::Vector3f;
using SCALAR = VEC3::Scalar;

SCALAR w_spline_kernel(const VEC3& r, const SCALAR h)
{
    const SCALAR q = r.squaredNorm() / h;
    if (q > 1.f)
        return 0;
    else if (q > .5f)
        return 2.f * pow((1 - q), 3.0);
    else
        return 6.f * q * q * (q - 1.f) + 1.f;
}

typedef struct {
    unsigned int i;
    SCALAR       m;
    VEC3         x;
    VEC3         v;
} Particle;

template <int n_x, int n_y, int n_z> class ParticleGrid {
    using GridPos = Eigen::Vector3<int>;

public:
    ParticleGrid(const SCALAR                 i_cell_size,
                 const std::vector<Particle>& i_particles,
                 const VEC3&                  grid_base = VEC3(0, 0, 0))
        : m_cell_size(i_cell_size), m_particles(i_particles), m_grid_base(grid_base)
    {
    }

    void update_neighbor_list(const SCALAR radius) { search_neighbor(radius); }

    std::vector<unsigned int>& get_neighbors(const unsigned int particle_index)
    {
        return m_neighbor_list[particle_index];
    }

private:
    inline unsigned int get_grid_index(const GridPos& grid_pos)
    {
        return grid_pos(0) + grid_pos(1) * n_x + grid_pos(2) * n_x * n_y;
    }

    template <typename T> bool in_range(T a, T b, T value) { return a <= value && value <= b; }

    inline bool in_grid(const GridPos& grid_pos)
    {
        return in_range<int>(0, n_x - 1, grid_pos(0)) && in_range<int>(0, n_y - 1, grid_pos(1)) &&
               in_range<int>(0, n_z - 1, grid_pos(2));
    }

    GridPos get_grid_pos(const VEC3& pos)
    {
        VEC3 rel_pos = (pos - m_grid_base) / m_cell_size;
        return GridPos(static_cast<int>(rel_pos(0)), static_cast<int>(rel_pos(1)), static_cast<int>(rel_pos(2)));
    }
    void search_neighbor(const SCALAR radius)
    {
        const int    n         = m_particles.size();
        const SCALAR r_squared = radius * radius;

        std::unordered_map<unsigned int, std::vector<unsigned int>> cell_particles;

        for (int i = 0; i < n; i++) {
            const int index = get_grid_index(get_grid_pos(m_particles[i].x));
            if (cell_particles.find(index) == cell_particles.end())
                cell_particles[index] = std::vector<unsigned int>{static_cast<unsigned int>(i)};
            else
                cell_particles[index].push_back(static_cast<unsigned int>(i));
        }

        const int range = static_cast<int>(radius / m_cell_size) + 1;

        for (int i = 0; i < n; i++) {
            printf("%d ", i);
            const GridPos grid_pos = get_grid_pos(m_particles[i].x);
            for (int x = -range; x <= range; x++)
                for (int y = -range; y <= range; y++)
                    for (int z = -range; z <= range; z++) {
                        const int i_x          = grid_pos(0) + x;
                        const int i_y          = grid_pos(1) + y;
                        const int i_z          = grid_pos(2) + z;
                        GridPos   tmp_grid_pos = GridPos(i_x, i_y, i_z);
                        if (!in_grid(tmp_grid_pos))
                            continue;

                        const int   cell_index = get_grid_index(GridPos(i_x, i_y, i_z));
                        const auto& list       = cell_particles[cell_index];

                        for (const int particle_index : list) {
                            const SCALAR squared_dist =
                                (m_particles[i].x - m_particles[particle_index].x).squaredNorm();
                            if (squared_dist < r_squared)
                                m_neighbor_list[i].push_back(particle_index);
                        }
                    }
        }
    }

private:
    std::vector<std::vector<unsigned int>> m_neighbor_list;
    const SCALAR                           m_cell_size;
    const std::vector<Particle>&           m_particles;
    const VEC3                             m_grid_base;
};

int main()
{
    std::vector<Particle> particles;
    constexpr SCALAR      dt               = 0.016f;
    constexpr int         num_particles    = 100;
    constexpr SCALAR      total_mass       = 3000.0f;
    constexpr SCALAR      container_width  = 1.0f;
    constexpr SCALAR      container_height = 2.0f;
    constexpr SCALAR      container_length = 3.0f;
    constexpr Vector3     container_pos{-container_width / 2, 0, -container_length / 2};
    constexpr Vector3     container_center{0, container_height / 2.f, 0};
    constexpr SCALAR      radius = 0.010f;

    particles.resize(num_particles);
    for (int i = 0; i < num_particles; i++) {
        particles[i].i = i;
        particles[i].m = total_mass / static_cast<SCALAR>(num_particles);
        particles[i].x = Raygen::cast_vec3_r2e(container_pos) + VEC3(container_width * Raygen::random_real<float>(),
                                                                     container_height * Raygen::random_real<float>(),
                                                                     container_length * Raygen::random_real<float>());
        particles[i].v = VEC3::Zero();
    }

    constexpr SCALAR cell_size = 0.05f;
    constexpr int    n_x       = container_width / cell_size;
    constexpr int    n_y       = container_height / cell_size;
    constexpr int    n_z       = container_length / cell_size;
    printf("Generating a 3D grid %dx%dx%d\n", n_x, n_y, n_z);

    ParticleGrid<n_x, n_y, n_z> particle_grid(cell_size, particles);
    particle_grid.update_neighbor_list(radius);

    constexpr int width  = 1024;
    constexpr int height = 768;
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    InitWindow(width, height, "Position Based Fluids");

    Camera3D camera;
    camera.fovy     = 45;
    camera.position = Vector3{4, 4, 4};
    camera.target   = container_center;
    camera.up       = Vector3{0, 1, 0};
    Raygen::OrbitCameraControl orbit_control(camera);

    Mesh                   sphere_mesh = GenMeshSphere(0.02, 5, 10);
    Raygen::ParticleSystem particle_system(num_particles, sphere_mesh, BLUE);
    Matrix*                transforms = (Matrix*)RL_CALLOC(num_particles, sizeof(Matrix));
    for (int i = 0; i < num_particles; i++) {
        transforms[i] = MatrixTranslate(particles[i].x(0), particles[i].x(1), particles[i].x(2));
    }

    while (!WindowShouldClose()) {
        Eigen::Vector2f cursor_delta_pos;
        if (IsMouseButtonDown(0) && IsKeyDown(KEY_LEFT_ALT)) {
            cursor_delta_pos = Raygen::get_mouse_delta() * GetFrameTime();
        } else {
            cursor_delta_pos = Eigen::Vector2f::Zero();
        }
        float mouse_wheel = GetMouseWheelMove();
        if (abs(mouse_wheel) + cursor_delta_pos.norm() > 0)
            orbit_control.update(camera, cursor_delta_pos, mouse_wheel);

        particle_system.update_camera(camera);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                DrawGrid(10, 1);
                particle_system.draw_instances(transforms);
                DrawCubeWires(container_center, container_width, container_height, container_length, RED);
            }
            EndMode3D();
        }
        EndDrawing();
    }

    CloseWindow();
}
