.. index::
   single: Forms; Fields; checkbox

.. note::

   * 対象バージョン：2.3+
   * 翻訳更新日：2014/05/18

checkbox フィールドタイプ
=========================

単一の入力チェックボックスを作成します。これは、常にブール値を持つフィールドに使用する必要があります。
チェックボックスがチェックされている場合 true に設定され、チェックボックスをオフにすると、 false に設定されます。

+------------------+------------------------------------------------------------------------+
| 対応するデータ型 | ``input`` ``checkbox`` フィールド                                      |
+------------------+------------------------------------------------------------------------+
| オプション       | - `value`_                                                             |
+------------------+------------------------------------------------------------------------+
| 上書きされた     | - `empty_data`_                                                        |
| オプション       | - `compound`_                                                          |
+------------------+------------------------------------------------------------------------+
| 継承された       | - `data`_                                                              |
| オプション       | - `required`_                                                          |
|                  | - `label`_                                                             |
|                  | - `label_attr`_                                                        |
|                  | - `read_only`_                                                         |
|                  | - `disabled`_                                                          |
|                  | - `error_bubbling`_                                                    |
|                  | - `error_mapping`_                                                     |
|                  | - `mapped`_                                                            |
+------------------+------------------------------------------------------------------------+
| 親タイプ         | :doc:`form </reference/forms/types/form>`                              |
+------------------+------------------------------------------------------------------------+
| クラス           | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+------------------+------------------------------------------------------------------------+

使用例
------

.. code-block:: php

    $builder->add('public', 'checkbox', array(
        'label'     => 'このエントリーを公開しますか?',
        'required'  => false,
    ));

フィールドオプション
--------------------

.. include:: /reference/forms/types/options/value.rst.inc

上書きされたオプション
----------------------

.. include:: /reference/forms/types/options/checkbox_empty_data.rst.inc

.. include:: /reference/forms/types/options/checkbox_compound.rst.inc

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

フォーム変数
------------

.. include:: /reference/forms/types/variables/check_or_radio_table.rst.inc

.. 2014/05/18 yositani2002 d49d12eaf265a5d6d32ac660c62f385d57261475
