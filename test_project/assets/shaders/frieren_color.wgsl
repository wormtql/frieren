struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) normal: vec3<f32>,
    @location(2) tex_coords: vec2<f32>,
    @location(3) tangent: vec3<f32>,
    @location(4) bitangent: vec3<f32>,
};

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(1) normal: vec3<f32>,
    @location(2) position: vec3<f32>,
};

struct TransformationUniform {
    model_matrix: mat4x4<f32>,
};

struct CameraUniform {
    world_space_camera_position: vec4<f32>,
    view_matrix: mat4x4<f32>,
    projection_matrix: mat4x4<f32>,
    projection_params: vec4<f32>,
};

struct LightUniform {
    light_intensity: vec4<f32>,
    light_position: vec4<f32>,
    orientation: vec4<f32>,
    params1: vec4<f32>,
    light_type: u32,
};

@group(0) @binding(0)
var<uniform> transform: TransformationUniform;

@group(0) @binding(1)
var<uniform> camera: CameraUniform;

@group(0) @binding(2)
var<uniform> light: LightUniform;

@vertex
fn vs_main(
    model: VertexInput,
    // @builtin(vertex_index) in_vertex_index: u32,
) -> VertexOutput {
    var out: VertexOutput;
    out.clip_position = camera.projection_matrix * camera.view_matrix * vec4<f32>(model.position, 1.0);
    // out.clip_position = camera.view_matrix * vec4<f32>(model.position, 1.0);
    // out.color = model.position * 0.5 + 0.5;
    // out.color = model.tangent * 0.5 + 0.5;

    out.position = model.position;
    out.normal = model.normal;
    return out;
}

// Fragment shader

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    // return vec4<f32>(normalize(in.normal), 1.0);

    let normal: vec3<f32> = normalize(in.normal);
    let light_dir = -normalize(light.orientation.xyz);
    let lambert = max(0f, dot(normal, light_dir));

    // let color = pow(lambert, 2.2);
    let color = lambert;

    return vec4<f32>(color, color, color, 1.0);
    // return vec4<f32>(normal * 0.5 + 0.5, 1.0);
    // return vec4<f32>(in.normal, 1.0);
}
