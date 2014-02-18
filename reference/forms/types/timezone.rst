.. index::
   single: Forms; Fields; timezone

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2014/02/16

timezone フィールドタイプ
=========================

``timezone`` タイプはすべてのタイムゾーンが選択可能な ``ChoiceType`` のサブセットです。

各タイムゾーンの "値" は、\ ``America/Chicago`` または ``Europe/Istanbul`` のような、タイムゾーン名です。

``choice`` タイプとは異なり、\ ``choices`` または ``choice_list`` オプションを特定する必要はなく、フィールドタイプとして自動的に大きなタイムゾーンのリストを使います。それらのオプションのどちらかを手動で設定することは *出来ます* が、その場合は ``choice`` タイプを直接使うべきです。

+-------------+------------------------------------------------------------------------+
| 対応するタグ| 様々なタグとして利用可能 (see :ref:`forms-reference-choice-tags`)      |
+-------------+------------------------------------------------------------------------+
| 上書きされた| - `choice_list`_                                                       |
| オプション  |                                                                        |
+-------------+------------------------------------------------------------------------+
| 継承された  | - `multiple`_                                                          |
| オプション  | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `empty_data`_                                                        |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `label_attr`_                                                        |
|             | - `data`_                                                              |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| 親タイプ    | :doc:`choice </reference/forms/types/choice>`                          |
+-------------+------------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType` |
+-------------+------------------------------------------------------------------------+

上書きされたオプション
----------------------

choice_list
~~~~~~~~~~~

**デフォルト**: :class:`Symfony\\Component\\Form\\Extension\\Core\\ChoiceList\\TimezoneChoiceList`

``Timezone`` タイプのデフォルトの ``choice_list `` は :phpmethod:`DateTimeZone::listIdentifiers` で返されるすべてのタイムゾーンとなり、大陸別に分類します。

継承されたオプション
--------------------

以下のオプションは :doc:`choice </reference/forms/types/choice>` タイプを継承しています:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2014/02/16 yositani2002 d9c6a853a84754e8fc8379bf61b70c02493e305f
