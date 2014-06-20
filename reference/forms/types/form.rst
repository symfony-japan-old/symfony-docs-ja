.. index::
   single: Forms; Fields; form

.. note::

    * 対象バージョン：2.3+
    * 翻訳更新日：2014/06/20

form フィールドタイプ
=====================

``form`` タイプを親タイプにもつすべてのタイプで利用可能なオプションを事前にいくつか定義しています。

+-----------+--------------------------------------------------------------------+
|オプション | - `data`_                                                          |
|           | - `data_class`_                                                    |
|           | - `empty_data`_                                                    |
|           | - `compound`_                                                      |
|           | - `required`_                                                      |
|           | - `label_attr`_                                                    |
|           | - `constraints`_                                                   |
|           | - `cascade_validation`_                                            |
|           | - `read_only`_                                                     |
|           | - `trim`_                                                          |
|           | - `mapped`_                                                        |
|           | - `property_path`_                                                 |
|           | - `max_length`_ (2.5 で非推奨)                                     |
|           | - `by_reference`_                                                  |
|           | - `error_bubbling`_                                                |
|           | - `inherit_data`_                                                  |
|           | - `error_mapping`_                                                 |
|           | - `invalid_message`_                                               |
|           | - `invalid_message_parameters`_                                    |
|           | - `extra_fields_message`_                                          |
|           | - `post_max_size_message`_                                         |
|           | - `pattern`_ (2.5 で非推奨)                                        |
|           | - `action`_                                                        |
|           | - `method`_                                                        |
+-----------+--------------------------------------------------------------------+
| 継承された| - `block_name`_                                                    |
| オプション| - `disabled`_                                                      |
|           | - `label`_                                                         |
|           | - `attr`_                                                          |
|           | - `translation_domain`_                                            |
|           | - `auto_initialize`_                                               |
+-----------+--------------------------------------------------------------------+
| 親クラス  | none                                                               |
+-----------+--------------------------------------------------------------------+
| クラス    | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FormType` |
+-----------+--------------------------------------------------------------------+

フィールドオプション
--------------------

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/data_class.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/compound.rst.inc

.. _reference-form-option-required:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/constraints.rst.inc

.. include:: /reference/forms/types/options/cascade_validation.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/trim.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. include:: /reference/forms/types/options/property_path.rst.inc

.. _reference-form-option-max_length:

.. include:: /reference/forms/types/options/max_length.rst.inc

.. include:: /reference/forms/types/options/by_reference.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/inherit_data.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/extra_fields_message.rst.inc

.. include:: /reference/forms/types/options/post_max_size_message.rst.inc

.. _reference-form-option-pattern:

.. include:: /reference/forms/types/options/pattern.rst.inc

.. include:: /reference/forms/types/options/action.rst.inc

.. include:: /reference/forms/types/options/method.rst.inc

継承されたオプション
--------------------

以下のオプションは :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BaseType` クラスで定義されています。
``BaseType`` クラスは ``form`` タイプと :doc:`button type </reference/forms/types/button>` の両方の親クラスですが、フォームタイプツリーの一部ではありません（すなわち、それ自身はフォームタイプとして利用できません）。

.. include:: /reference/forms/types/options/block_name.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/attr.rst.inc

.. include:: /reference/forms/types/options/translation_domain.rst.inc

.. include:: /reference/forms/types/options/auto_initialize.rst.inc


.. 2013/11/24 yositani2002 b675661199d466be4b6cb6f70d16aa1e3574c789
.. 2014/06/20 yositani2002 21a4b9df8f4819b73ced858f323e835e10dfd89c