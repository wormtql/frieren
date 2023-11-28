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
    @location(3) uv: vec2<f32>,
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

@group(1) @binding(1)
var r_sf1_texture: texture_2d<f32>;

@group(1) @binding(2)
var r_sf1_texture_sampler: sampler;

@group(1) @binding(3)
var r_sf1_bar_texture: texture_1d<f32>;

fn get_one_minus_r_sf1(dir: vec3<f32>, normal: vec3<f32>, alpha: f32) -> f32 {
    let cos_theta = dot(dir, normal);
//    let uv = vec2<f32>(cos_theta, 1.0 - alpha);
    let uv = vec2<f32>(cos_theta, alpha);
    let value = textureSample(r_sf1_texture, r_sf1_texture_sampler, uv);
    return value.x;
}

fn get_r_sf1_bar(alpha: f32) -> f32 {
    let value = textureSample(r_sf1_bar_texture, r_sf1_texture_sampler, alpha);
    return value.r;
}

fn multi_scatter_term(light: vec3<f32>, view: vec3<f32>, normal: vec3<f32>, f0: vec3<f32>, alpha: f32) -> vec3<f32> {
    let f_bar: vec3<f32> = 0.95238 * f0 + 0.047619;
    let rsf1_light: f32 = get_one_minus_r_sf1(light, normal, alpha);
    let rsf1_view: f32 = get_one_minus_r_sf1(view, normal, alpha);
    let rsf1_bar: f32 = get_r_sf1_bar(alpha);
    let x: f32 = f_bar.x * rsf1_bar / (3.14 * (1.0 - rsf1_bar) * (1.0 - f_bar.x * (1.0 - rsf1_bar)));
//    let x: f32 = 1.0 * rsf1_bar / (3.14 * (1.0 - rsf1_bar) * (1.0 - 1.0 * (1.0 - rsf1_bar)));
    let y: f32 = f_bar.y * rsf1_bar / (3.14 * (1.0 - rsf1_bar) * (1.0 - f_bar.y * (1.0 - rsf1_bar)));
    let z: f32 = f_bar.z * rsf1_bar / (3.14 * (1.0 - rsf1_bar) * (1.0 - f_bar.z * (1.0 - rsf1_bar)));
    let temp = vec3<f32>(x, y, z);

//    let vis = step(0.0, dot(light, normal)) * step(0.0, dot(view, normal));

//    return f_bar;
//    return vec3<f32>(rsf1_light, 0.0, 0.0);
//    return vec3<f32>(rsf1_view, 0.0, 0.0);
//    return vec3<f32>(rsf1_bar, 0.0, 0.0);
//    return vec3<f32>(x, 0.0, 0.0);
//    return vec3<f32>(f_bar.y, 0.0, 0.0);
//    return temp;
    return temp * rsf1_light * rsf1_view;
//    return vec3<f32>(rsf1_bar, 0.0f, 0.0f);
//    return vec3<f32>(alpha, 0.0f, 0.0f);
//    return view;
}

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

fn smith_g1_ggx(m: vec3<f32>, view: vec3<f32>, alpha_g: f32, normal: vec3<f32>) -> f32 {
    let temp: f32 = max(0f, dot(m, view));
    let ns = max(0f, dot(normal, view));
    // let a: f32 = ns / (alpha_g * sqrt(1.0f - ns * ns));
    let a: f32 = temp / (alpha_g * sqrt(1.0f - temp * temp));
    return temp / (1.0f + lambda_ggx(a));
}

fn smith_g2_ggx_simple(light: vec3<f32>, view: vec3<f32>, m: vec3<f32>, alpha_g: f32, normal: vec3<f32>) -> f32 {
    return smith_g1_ggx(m, view, alpha_g, normal) * smith_g1_ggx(m, light, alpha_g, normal);
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
    let alpha = shader_properties.alpha;
    let g2: f32 = smith_g2_ggx_simple(light, view, half, alpha, normal);
    // let g2: f32 = smith_g2_ggx_simple(light, view, normal, shader_properties.alpha);
    let d: f32 = ndf_ggx(half, normal, shader_properties.alpha);
    let directional_albedo: f32 = 1f - get_one_minus_r_sf1(view, normal, alpha);

    let g2_with_denom = smith_g2_height_field_with_denom(light, view, half, shader_properties.alpha);

    // return vec3<f32>(max(0.0f, dot(light, normal)), 0f, 0f);
    // return normal;
    // return view;
    // return light;
    // return f;
    // return f0;
    // return vec3<f32>(d, 0f, 0f);
    // return f * d;
    // return f * d * g2;
    // return vec3<f32>(g2, 0f, 0f);

    // let diffuse_albedo = vec3<f32>(1.0, 1.0, 1.0);
    let diffuse_albedo = vec3<f32>(1f, 1f, 1f) - f0;
    // let diffuse_color = f0;
    let diffuse_energy = vec3<f32>(1f, 1f, 1f) - f;
    let diffuse = diffuse_energy * diffuse_albedo / 3.14;

    let nom: vec3<f32> = f * g2 * d;
    let nl = abs(dot(normal, light));
    let nv = abs(dot(normal, view));
    // let specular = nom / (4.0f * nl * nv + 0.001);
    let specular = f * d * g2_with_denom;

    let multi_scatter = multi_scatter_term(light, view, normal, f0, alpha);

//    return view;
//    return vec3<f32>(1f - directional_albedo, 0f, 0f);
    return specular + multi_scatter;
//    return multi_scatter;
    // return vec3<f32>(nv, 0f, 0f);
    // return vec3<f32>(nv * nl * 4f, 0f, 0f);
//    return specular;
    // return vec3<f32>(g2, 0f, 0f);
//     return specular + diffuse;
    // return diffuse;
    // return vec3<f32>(1f, 1f, 1f) - f;
//    return vec3<f32>(alpha, 0f, 0f);
}

@vertex
fn vs_main(
    model: VertexInput,
    // @builtin(vertex_index) in_vertex_index: u32,
) -> VertexOutput {
    var out: VertexOutput;

    let world_position = transform.model_matrix * vec4<f32>(model.position, 1.0f);
    let world_normal = transform.model_matrix * vec4<f32>(model.normal, 0.0f);

    out.clip_position = camera.projection_matrix * camera.view_matrix * world_position;
    // out.clip_position = camera.view_matrix * vec4<f32>(model.position, 1.0);
    // out.color = model.position * 0.5 + 0.5;
    // out.color = model.tangent * 0.5 + 0.5;

    out.position = world_position.xyz;
    out.normal = world_normal.xyz;
    out.uv = model.tex_coords;
    return out;
}

// Fragment shader

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    let normal: vec3<f32> = normalize(in.normal);
    let light_dir = -normalize(light.orientation.xyz);

    let camera_position = camera.world_space_camera_position.xyz;
    let view_dir: vec3<f32> = normalize(camera_position - in.position);
    let specular: vec3<f32> = brdf(light_dir, view_dir, normal);

//    return vec4<f32>(in.position, 1.0f);
    // return vec4<f32>(normal, 1.0f);
    // return vec4<f32>(light_dir, 1.0f);
//     return vec4<f32>(view_dir, 1.0f);
    // return vec4<f32>(shader_properties.alpha, 0f, 0f, 1.0f);
    // return vec4<f32>(shader_properties.refraction_index2 / 2f, 1.0f);

//    let color = textureSample(r_sf1_texture, r_sf1_texture_sampler, in.uv);
//    let color2 = textureSample(r_sf1_bar_texture, r_sf1_texture_sampler, in.uv.x);

//    return vec4<f32>(color2.xyz, 1f);
    return vec4<f32>(specular, 1.0f);
//    return vec4<f32>(camera_position, 1.0f);
}
