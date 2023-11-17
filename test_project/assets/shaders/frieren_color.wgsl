struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) tex_coords: vec2<f32>,
    @location(2) normal: vec3<f32>,
    @location(3) tangent: vec3<f32>,
    @location(4) bitangent: vec3<f32>,
};

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) color: vec3<f32>,
};

struct TransformationUniform {
    world_space_camera_position: vec4<f32>,
    view_matrix: mat4x4<f32>,
    projection_matrix: mat4x4<f32>,
    projection_params: vec4<f32>,
};

@group(0) @binding(0)
var<uniform> transformation: TransformationUniform;

@vertex
fn vs_main(
    model: VertexInput,
    @builtin(vertex_index) in_vertex_index: u32,
) -> VertexOutput {
    var out: VertexOutput;
    out.clip_position = transformation.projection_matrix * transformation.view_matrix * vec4<f32>(model.position, 1.0);
    // out.clip_position = transformation.view_matrix * vec4<f32>(model.position, 1.0);
    // out.color = model.position * 0.5 + 0.5;
    // out.color = model.tangent * 0.5 + 0.5;
    out.color = model.normal * 0.5 + 0.5;
    return out;
}

// Fragment shader

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(in.color, 1.0);
}
