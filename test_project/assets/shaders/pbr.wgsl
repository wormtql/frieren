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

struct PBRParams {
    f0: vec3<f32>,
    // refraction_index1: vec3<f32>,
    // refraction_index2: vec3<f32>,
    alpha: f32,
};

@group(0) @binding(0)
var<uniform> transform: TransformationUniform;

@group(0) @binding(1)
var<uniform> camera: CameraUniform;

@group(0) @binding(2)
var<uniform> light: LightUniform;

@group(1) @binding(0)
var<uniform> shader_properties: PBRParams;

// schlick approx
fn fresnel(normal: vec3<f32>, light: vec3<f32>, f0: vec3<f32>) -> vec3<f32> {
    let temp: f32 = max(dot(normal, light), 0.0f);
    return f0 + (1.0f - f0) * pow(1.0f - temp, 5.0f);
}

// GGX
fn ndf_ggx(m: vec3<f32>, normal: vec3<f32>, alpha_g: f32) -> f32 {
    let t: f32 = dot(normal, m);
    if t <= 0.0f {
        return 0f;
    } else {
        let a2: f32 = alpha_g * alpha_g;
        let x: f32 = (1.0f + t * t * (a2 - 1.0f));
        let denom = 3.14 * x * x;
        return a2 / denom;
    }
}

fn lambda_ggx(a: f32) -> f32 {
    let temp = 1f + 1f / (a * a);
    return (-1f + sqrt(temp)) / 2f;
}

fn smith_g1_ggx(m: vec3<f32>, view: vec3<f32>, alpha_g: f32) -> f32 {
    let temp: f32 = max(0f, dot(m, view));
    let a: f32 = temp / (alpha_g * sqrt(1.0f - temp * temp));
    return temp / (1.0f + lambda_ggx(a));
}

fn smith_g2_ggx_simple(light: vec3<f32>, view: vec3<f32>, m: vec3<f32>, alpha_g: f32) -> f32 {
    return smith_g1_ggx(m, view, alpha_g) * smith_g1_ggx(m, light, alpha_g);
}

fn smith_g2_height_field_with_denom(light: vec3<f32>, view: vec3<f32>, m: vec3<f32>, alpha_g: f32) -> f32 {
    let ui = saturate(dot(m, light));
    let uo = saturate(dot(m, view));
    let a2 = alpha_g * alpha_g;

    let x1 = uo * sqrt(a2 + ui * (ui - a2 * ui));
    let x2 = ui * sqrt(a2 + uo * (uo - a2 * uo));
    return 0.5 / (x1 + x2);
}

fn get_f0(index1: vec3<f32>, index2: vec3<f32>) -> vec3<f32> {
    let a = (index1.x - index2.x) / (index1.x + index2.x);
    let b = (index1.y - index2.y) / (index1.y + index2.y);
    let c = (index1.z - index2.z) / (index1.z + index2.z);
    return vec3<f32>(a * a, b * b, c * c);
}

fn brdf(light: vec3<f32>, view: vec3<f32>, normal: vec3<f32>) -> vec3<f32> {
    let half: vec3<f32> = normalize(light + view);
    // let f0: vec3<f32> = get_f0(shader_properties.refraction_index1, shader_properties.refraction_index2);
    let f0 = shader_properties.f0;
    let f: vec3<f32> = fresnel(half, light, f0);
    let g2: f32 = smith_g2_ggx_simple(light, view, half, shader_properties.alpha);
    // let g2: f32 = smith_g2_ggx_simple(light, view, normal, shader_properties.alpha);
    let d: f32 = ndf_ggx(half, normal, shader_properties.alpha);

    let g2_with_denom = smith_g2_height_field_with_denom(light, view, half, shader_properties.alpha);

    // return vec3<f32>(max(0.0f, dot(light, normal)), 0f, 0f);
    // return normal;
    // return view;
    // return light;
    // return f;
    // return f0;
    // return vec3<f32>(d, 0f, 0f);
    // return f * d;
    return f * d * g2;
    // return vec3<f32>(g2, 0f, 0f);

    // // let diffuse_color = vec3<f32>(1.0, 0.4, 0.2);
    // let diffuse_color = f0;
    // let diffuse = (vec3<f32>(1f, 1f, 1f) - f) * diffuse_color / 3.14;

    // let nom: vec3<f32> = f * g2 * d;
    // let nl = abs(dot(normal, light));
    // let nv = abs(dot(normal, view));
    // let specular = nom / (4.0f * nl * nv + 0.001);
    // // let specular = f * d * g2_with_denom;

    // // return vec3<f32>(nv, 0f, 0f);
    // // return vec3<f32>(nv * nl * 4f, 0f, 0f);
    // return specular;
    // // return vec3<f32>(g2, 0f, 0f);
    // // return specular + diffuse;
}

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
    let normal: vec3<f32> = normalize(in.normal);
    let light_dir = -normalize(light.orientation.xyz);

    let view_dir: vec3<f32> = normalize(camera.world_space_camera_position.xyz - in.position);
    let specular: vec3<f32> = brdf(light_dir, view_dir, normal);

    // return vec4<f32>(normal, 1.0f);
    // return vec4<f32>(light_dir, 1.0f);
    // return vec4<f32>(view_dir, 1.0f);
    // return vec4<f32>(shader_properties.alpha, 0f, 0f, 1.0f);
    // return vec4<f32>(shader_properties.refraction_index2 / 2f, 1.0f);

    return vec4<f32>(specular, 1.0f);
}
