{% macro header_error(api, header_name, name) %}
#ifdef {{ header_name }}
    #error {{ name }} header already included (API: {{ api }}), remove previous include!
#endif
#define {{ header_name }} 1
{% endmacro %}


{% macro context_arg(suffix='', def='') -%}
{{ 'Glad' + feature_set.api|api + 'Context *context' + suffix if options.mx else def }}
{%- endmacro %}


{% macro protect(symbol) %}
{% set protections = spec.protections(symbol, feature_set.api, feature_set.profile, feature_set=feature_set) %}
{% if protections %}
#if {{ protections|map('defined')|join(' || ') }}
{% endif %}
{{ caller() }}
{%- if protections %}

#endif
{% endif %}
{% endmacro %}

{% macro write_feature_information(extensions, with_runtime=True) %}
{% for extension in extensions %}
{% call protect(extension) %}
#define {{ extension.name }} 1
{% if with_runtime %}
GLAD_API_CALL int GLAD_{{ extension.name }};
{% endif %}
{% endcall %}
{% endfor %}
{% endmacro %}


{% macro write_types(types) %}
{# we assume the types are sorted correctly #}
{% for type in types %}
{{ write_type(type) }}
{% endfor %}
{% endmacro %}

{% macro write_type(type) %}
{% call protect(type) %}
{% if type.category == 'enum' -%}
{%- if type.enums_for(feature_set) -%}
typedef enum {{ type.name }} {
{% for member in type.enums_for(feature_set) %}
    {{ member.name }} = {{ member.alias if member.alias else member.value }}{{ ',' if not loop.last }}
{% endfor %}
} {{ type.name }};
{%- endif -%}
{%- elif type.category in ('struct', 'union') -%}
typedef {{ type.category }} {{ type.name }} {% if type.members %}{
{% for member in type.members %}
    {{ member.type.raw }};
{% endfor %}
}{% endif %} {{ type.name }};
{%- elif type.alias %}
#define {{ type.name }} {{ type.alias }}
{%- elif type.raw.strip() -%}
{{ type.raw.strip() }}
{%- endif %}
{% endcall %}
{% endmacro %}

{% macro write_enumerations(enumerations) %}
{% for enum in enumerations %}
{% call protect(enum) %}
#define {{ enum.name }} {{ enum.alias if enum.alias else enum.value }}
{% endcall %}
{% endfor %}
{% endmacro %}

{% macro write_function_definitions(commands) %}
{% for command in commands %}
{% call protect(command) %}
{{ command.proto.ret|type_to_c }} {{ command.name }}({{ command.params|params_to_c }});
{% endcall %}
{% endfor %}
{% endmacro %}

{% macro write_function_typedefs(commands) %}
{% for command in commands %}
{% call protect(command) %}
typedef {{ command.proto.ret|type_to_c }} (GLAD_API_PTR *{{ command.name|pfn }})({{ command.params|params_to_c }});
{% endcall %}
{% endfor %}
{% endmacro %}

{% macro write_function_declarations(commands, debug=False) %}
{% for command in commands %}
{% call protect(command) %}
GLAD_API_CALL {{ command.name|pfn }} glad_{{ command.name }};
{% if debug %}
GLAD_API_CALL {{ command.name|pfn }} glad_debug_{{ command.name }};
#define {{ command.name }} glad_debug_{{ command.name }}
{% else %}
#define {{ command.name }} glad_{{ command.name }}
{% endif %}
{% endcall %}
{% endfor %}
{% endmacro %}
